// Copyright XSleepy. All Rights Reserved.


#include "Character/SleepyFireEnemy.h"

void ASleepyFireEnemy::HighlightActor()
{
	//GetBodyMesh()->SetRenderCustomDepth(true);
	//GetBodyMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

}

void ASleepyFireEnemy::UnHighlightActor()
{
	
}

void ASleepyFireEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo(nullptr);
}

void ASleepyFireEnemy::InitAbilityActorInfo(AController* NewController)
{
	if(NewController==nullptr)
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	Cast<USleepyFireAbilitySystemComponent>(GetAbilitySystemComponent())->AbilityActorInfoSet();
}
