// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/SleepyFireAttributeSet.h"
#include "SleepyFireManaSet.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API USleepyFireManaSet : public USleepyFireAttributeSet
{
	GENERATED_BODY()
public:
	USleepyFireManaSet();

public:
	ATTRIBUTE_ACCESSORS(USleepyFireManaSet, Mana);
	ATTRIBUTE_ACCESSORS(USleepyFireManaSet, MaxMana);

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
		void OnRep_Mana(const FGameplayAttributeData& OldValue)const;
	UFUNCTION()
		void OnRep_MaxMana(const FGameplayAttributeData& OldValue)const;

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="SleepyFire||Mana",meta=(AllowPrivateAccess=true))
		FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "SleepyFire||MaxMana", meta = (AllowPrivateAccess = true))
		FGameplayAttributeData MaxMana;
};
