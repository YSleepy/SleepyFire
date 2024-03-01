// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/SleepyFireBaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "SleepyFireEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API ASleepyFireEnemy : public ASleepyFireBaseCharacter,public IEnemyInterface
{
	GENERATED_BODY()
public:
	/** EnemyInterface **/
	virtual void HighlightActor()override;
	virtual void UnHighlightActor() override;
	/** End EnemyInterface **/
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo(AController* NewController) override;
};
