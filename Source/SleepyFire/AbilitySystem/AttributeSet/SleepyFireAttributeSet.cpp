// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepyFireAttributeSet.h"



USleepyFireAttributeSet::USleepyFireAttributeSet()
{
}

UWorld* USleepyFireAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);
	return Outer->GetWorld();
}

USleepyFireAbilitySystemComponent* USleepyFireAttributeSet::GetSleepyFireAbilitySystemComponent() const
{
	return Cast<USleepyFireAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
