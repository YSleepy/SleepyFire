// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystem/SleepyFireAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/SleepyFireHealthSet.h"
#include"AbilitySystem/AttributeSet/SleepyFireManaSet.h"
#include "AbilitySystemInterface.h"
#include "Character/SleepyFireAttributeComponent.h"

#include "SleepyFirePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API ASleepyFirePlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ASleepyFirePlayerState();
	// 通过 IAbilitySystemInterface 继承
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	USleepyFireHealthSet* GetHealthSet()const;
	USleepyFireManaSet* GetManaSet()const;
private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr <USleepyFireAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USleepyFireHealthSet>HealthSet;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USleepyFireManaSet>ManaSet;

};
