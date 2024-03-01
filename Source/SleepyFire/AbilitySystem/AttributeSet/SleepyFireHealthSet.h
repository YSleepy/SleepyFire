// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SleepyFireAttributeSet.h"
#include "AbilitySystem/SleepyFireAbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "SleepyFireHealthSet.generated.h"

/**
 * 
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageImmunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_DamageSelfDestruct);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_FellOutOfWorld);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SleepyFire_Damage_Message);

UCLASS()
class SLEEPYFIRE_API USleepyFireHealthSet : public USleepyFireAttributeSet
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	USleepyFireHealthSet();

	ATTRIBUTE_ACCESSORS(USleepyFireHealthSet, Health);
	ATTRIBUTE_ACCESSORS(USleepyFireHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(USleepyFireHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(USleepyFireHealthSet, Damage);

	mutable FSleepyFireAttributeEvent OnOutOfHealth;
public:

	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldValue)const;

	UFUNCTION()
		void OnRep_MaxHealth(const FGameplayAttributeData& OldValue)const;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	//Clamp changed value
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "SleepyFire||Health", Meta = ( AllowPrivateAccess = true))
		FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "SleepyFire||Health", Meta = ( AllowPrivateAccess = true))
		FGameplayAttributeData MaxHealth;
	//跟踪Health值为0
	bool bOutOfHealth;
private:

	UPROPERTY(BlueprintReadOnly, Category = "SleepyFire||Health", Meta = (AllowPrivateAccess = true))
		FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category = "SleepyFire||Health", Meta = (AllowPrivateAccess = true))
		FGameplayAttributeData Damage;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props)const;
	void PrintLogForHealth(FString CalledFun,float OldValue,float NewValue)const;
};
