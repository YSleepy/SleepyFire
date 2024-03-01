// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SleepyFireAbilitySystemComponent.h"

#include "KismetCastingUtils.h"

//（Server and Client）在构造完AbilitySystemComponent后通常回调要此函式完成 效果应用绑定。
void USleepyFireAbilitySystemComponent::AbilityActorInfoSet()
{
	//Bind EffectApplied to OnGameplayEffectAppliedDelegateToSelf (Effect applied to self)
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USleepyFireAbilitySystemComponent::EffectApplied);
}

void USleepyFireAbilitySystemComponent::EffectApplied(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);
	//EffectAssetTags.Broadcast(TagContainer);//服务端
	ClientEffectAssetTagsBroadcast(TagContainer);

}



void USleepyFireAbilitySystemComponent::ClientEffectAssetTagsBroadcast_Implementation(const FGameplayTagContainer& TagContainer)
{
	EffectAssetTags.Broadcast(TagContainer);
}
