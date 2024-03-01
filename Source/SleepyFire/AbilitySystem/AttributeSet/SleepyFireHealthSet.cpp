// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepyFireHealthSet.h"
#include "SleepyFireGameplayTags.h"
#include "Message/SleepyFireVerbMessage.h"
#include <Net/UnrealNetwork.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayMessageSubsystem.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_Damage, "Gameplay.Damage");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageSelfDestruct, "Gameplay.Damage.SelfDestruct");
UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_FellOutOfWorld, "Gameplay.Damage.FellOutOfWorld");
UE_DEFINE_GAMEPLAY_TAG(TAG_SleepyFire_Damage_Message, "SleepyFire.Damage.Message");

void USleepyFireHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USleepyFireHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USleepyFireHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

USleepyFireHealthSet::USleepyFireHealthSet():Health(100.f),MaxHealth(100.f)
{
	bOutOfHealth = false;
}


bool USleepyFireHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	// 调用父类的 PreGameplayEffectExecute 函数，如果返回 false，则直接返回 false
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}
	PrintLogForHealth("PreGameplayEffectExecute",0,0);
	// 如果是伤害属性
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// 如果伤害值大于 0
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			// 检查是否是自毁伤害
			const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);

			// 如果目标具有伤害免疫标签并且不是自毁伤害
			if (Data.Target.HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity) && !bIsDamageFromSelfDestruct)
			{
				// 不扣除任何生命值放弃属性值的变更
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}

#if !UE_BUILD_SHIPPING
			// 检查上帝模式作弊，无限生命在下面检查
			if (Data.Target.HasMatchingGameplayTag(SleepyFireGameplayTags::Cheat_GodMode) && !bIsDamageFromSelfDestruct)
			{
				// 不扣除任何生命值
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
#endif // #if !UE_BUILD_SHIPPING
		}
	}

	// 返回 true 表示允许修改生效
	return true;
}

void USleepyFireHealthSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	//if uses EffectActor ,&Source == &Target 
	UE_LOG(LogTemp, Warning, TEXT("Source:%p,Target:%p"), Props.SourceASC, &Data.Target);
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void USleepyFireHealthSet::PrintLogForHealth(FString CalledFun,float OldValue,float NewValue)const
{
	UE_LOG(LogTemp, Warning, TEXT("%s:GetHealth:%f||OldValue:%f,NewValue:%f"), *CalledFun, GetHealth(), OldValue, NewValue);
}

void USleepyFireHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	PrintLogForHealth("PostGameplayEffectExecute",0,0);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

	const bool bIsDamageFromSelfDestruct = Data.EffectSpec.GetDynamicAssetTags().HasTagExact(TAG_Gameplay_DamageSelfDestruct);
	float MinimumHealth = 0.0f;

#if !UE_BUILD_SHIPPING
	// 作弊和无敌下不允许死亡
	if (!bIsDamageFromSelfDestruct &&
		(Data.Target.HasMatchingGameplayTag(SleepyFireGameplayTags::Cheat_GodMode) || Data.Target.HasMatchingGameplayTag(SleepyFireGameplayTags::Cheat_UnlimitedHealth)))
	{
		MinimumHealth = 1.0f;
	}
#endif // #if !UE_BUILD_SHIPPING

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())//如果是Damage
	{
		// 发送信息到其他系统
		if (Data.EvaluatedData.Magnitude > 0.0f)//未被clamp的数据，具有不可靠性
		{
			FSleepyFireVerbMessage Message;
			Message.Verb = TAG_SleepyFire_Damage_Message;
			Message.Instigator = Data.EffectSpec.GetEffectContext().GetEffectCauser();
			Message.InstigatorTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
			Message.Target = GetOwningActor();
			Message.TargetTags = *Data.EffectSpec.CapturedTargetTags.GetAggregatedTags();
			//@TODO: Fill out context tags, and any non-ability-system source/instigator tags
			//@TODO: Determine if it's an opposing team kill, self-own, team kill, etc...
			Message.Magnitude = Data.EvaluatedData.Magnitude;

			UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSystem.BroadcastMessage(Message.Verb, Message);
		}

		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())//治疗
	{
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())//直接修改Health
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)//Health<=0
	{
		if (OnOutOfHealth.IsBound())
		{
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();//Gameplay 效果的上下文信息
			AActor* Instigator = EffectContext.GetOriginalInstigator();//施法者
			AActor* Causer = EffectContext.GetEffectCauser();//受击者

			OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
		}
	}

	bOutOfHealth = (GetHealth() <= 0.0f);
}

void USleepyFireHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	PrintLogForHealth("PreAttributeBaseChange",0,NewValue);

	ClampAttribute(Attribute, NewValue);
}


void USleepyFireHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	PrintLogForHealth("PreAttributeChange",0,NewValue);

	ClampAttribute(Attribute, NewValue);
}


void USleepyFireHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	PrintLogForHealth("PostAttributeChange",OldValue,NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// 当前生命不得大于最大生命
		if (GetHealth() > NewValue)
		{
			USleepyFireAbilitySystemComponent* LyraASC = GetSleepyFireAbilitySystemComponent();
			check(LyraASC);

			LyraASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}



void USleepyFireHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// 不要让生命值为负值或超过最大生命值。
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// 不允许最大生命小于1
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}




void USleepyFireHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)const
{
	//处理客户端预测性修改的属性,系统可能做一些回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(USleepyFireHealthSet, Health, OldValue);
}

void USleepyFireHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USleepyFireHealthSet, MaxHealth, OldValue);
}
