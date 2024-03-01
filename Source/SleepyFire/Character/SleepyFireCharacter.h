// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/SleepyFireBaseCharacter.h"
#include "SleepyFireCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API ASleepyFireCharacter : public ASleepyFireBaseCharacter
{
	GENERATED_BODY()
public:
	// Init ability actor info for the server when character is controlled
	virtual void PossessedBy(AController* NewController) override;
	// Init ability actor info for the client when PlayerState is Copied
	virtual void OnRep_PlayerState() override;
private:
	virtual void InitAbilityActorInfo(AController* NewController) override;
};
