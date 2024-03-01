// Copyright XSleepy. All Rights Reserved.


#include "Character/SleepyFireCharacter.h"
#include "SleepyFireCharacter.h"

void ASleepyFireCharacter::PossessedBy(AController* NewController)
{
	//ASleepyFireBaseCharacter::PossessedBy(NewController);
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy is Call %p"), NewController);
	InitAbilityActorInfo(NewController);
}

void ASleepyFireCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerState is Call"));
	InitAbilityActorInfo(GetController());
}

void ASleepyFireCharacter::InitAbilityActorInfo(AController* NewController)
{
	Super::InitAbilityActorInfo(NewController);
}
