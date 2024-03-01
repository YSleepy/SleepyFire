// Copyright XSleepy. All Rights Reserved.


#include "Player/SleepyFirePlayerState.h"

#include "AbilitySystem/AttributeSet/SleepyFireAttributeSet.h"
#include <AbilitySystem/AttributeSet/SleepyFireHealthSet.h>
#include <AbilitySystem/AttributeSet/SleepyFireManaSet.h>

ASleepyFirePlayerState::ASleepyFirePlayerState()
{
	this->AbilitySystemComponent = CreateDefaultSubobject<USleepyFireAbilitySystemComponent>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	this->HealthSet = CreateDefaultSubobject<USleepyFireHealthSet>("HealthSet");
	this->ManaSet = CreateDefaultSubobject<USleepyFireManaSet>("ManaSet");

	NetUpdateFrequency = 100.f;//AbilitySystemComponent 需要一个较高的更新频率以同步客户端。
}

UAbilitySystemComponent* ASleepyFirePlayerState::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}


USleepyFireHealthSet* ASleepyFirePlayerState::GetHealthSet()const
{
	return this->HealthSet;
}

USleepyFireManaSet* ASleepyFirePlayerState::GetManaSet()const
{
	return this->ManaSet;
}

