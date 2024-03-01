// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/SleepyFireAttributeComponent.h"
#include <Player/SleepyFirePlayerState.h>
#include "GameFramework/Actor.h"
#include "UI/HUD/SleepyFireHUD.h"

// Sets default values for this component's properties
USleepyFireAttributeComponent::USleepyFireAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// ...
}



// Called when the game starts
void USleepyFireAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	//CompOwner = nullptr;
	//PlayerController = nullptr;
}


// Called every frame
void USleepyFireAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

USleepyFireAbilitySystemComponent* USleepyFireAttributeComponent::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

float USleepyFireAttributeComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float USleepyFireAttributeComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float USleepyFireAttributeComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();

		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}

	return 0.0f;
}

float USleepyFireAttributeComponent::GetMana() const
{
	return (ManaSet ? ManaSet->GetMana() : 0.0f);
}

float USleepyFireAttributeComponent::GetMaxMana() const
{
	return (ManaSet ? ManaSet->GetMaxMana() : 0.0f);
}

float USleepyFireAttributeComponent::GetManaNormalized() const
{
	if (ManaSet)
	{
		const float Mana = ManaSet->GetMana();
		const float MaxMana = ManaSet->GetMaxMana();

		return ((MaxMana > 0.0f) ? (Mana / MaxMana) : 0.0f);
	}

	return 0.0f;
}

void USleepyFireAttributeComponent::InitAbilityActorInfo(AController* NewController)
{

	while (CompOwner == nullptr)
	{
		CompOwner = Cast<ACharacter>(GetOwner());
	}
	ASleepyFirePlayerState* SleepyFirePlayerState = CompOwner->GetPlayerState<ASleepyFirePlayerState>();
	check(SleepyFirePlayerState);
	SleepyFirePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(SleepyFirePlayerState, CompOwner);
	AbilitySystemComponent = Cast<USleepyFireAbilitySystemComponent>(SleepyFirePlayerState->GetAbilitySystemComponent());
	AbilitySystemComponent->AbilityActorInfoSet();

	HealthSet = SleepyFirePlayerState->GetHealthSet();
	ManaSet = SleepyFirePlayerState->GetManaSet();

	if (NewController)//Server Or Client Autonomous Proxy
	{
		PlayerController = Cast<APlayerController>(NewController);
		if(ASleepyFireHUD* SleepyFireHUD = Cast<ASleepyFireHUD>(PlayerController->GetHUD()))
		{
			SleepyFireHUD->InitOverlay(PlayerController, SleepyFirePlayerState, AbilitySystemComponent, HealthSet);
		}
	}
}
