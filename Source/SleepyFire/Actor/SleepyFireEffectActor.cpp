// Copyright XSleepy. All Rights Reserved.


#include "Actor/SleepyFireEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AttributeSet/SleepyFireHealthSet.h"
#include "SleepyFireEffectActor.h"
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
ASleepyFireEffectActor::ASleepyFireEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));
}

// Called when the game starts or when spawned
void ASleepyFireEffectActor::BeginPlay()
{
	Super::BeginPlay();

}

void ASleepyFireEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystemComponent == nullptr)return;
	checkf(EffectClass,TEXT("EffectClass is NULL,Please fill EffectClass"));
	//EffectContextHandle 效果上下文，额外信息：施法者，目标，源能力，预测键等
	FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(EffectClass, ActorEffectLevel, EffectContextHandle);

	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*(EffectSpecHandle.Data.Get()));
	//持续策略是Infinite,并移除策略是RemoveOnOverlap
	const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bIsInfinite&& this->InfinityEffectRemovePolicy==EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		this->ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
	}
}

void ASleepyFireEffectActor::OnOverlap(AActor* TargetActor)
{
	if(this->InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if(this->DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if(this->InfinityEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
	}
}

void ASleepyFireEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (this->InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if (this->DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (this->InfinityEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
	}
	//if Infinity and Effect remove policy is RemoveOnEndOverlap 
	if(this->InfinityEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC))return;
		TArray<FActiveGameplayEffectHandle>HandlesToRemove;//Prevent iterator failure
		for(TTuple<FActiveGameplayEffectHandle,UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if(TargetASC==HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for(FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}


