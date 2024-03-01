// Copyright XSleepy. All Rights Reserved.


#include "SleepyFireManaSet.h"

#include "Net/UnrealNetwork.h"

USleepyFireManaSet::USleepyFireManaSet():Mana(100.f),MaxMana(100.f)
{
}

void USleepyFireManaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(USleepyFireManaSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USleepyFireManaSet, MaxMana, COND_None, REPNOTIFY_Always);

}

void USleepyFireManaSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USleepyFireManaSet, Mana, OldValue);
}

void USleepyFireManaSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USleepyFireManaSet, MaxMana, OldValue);
}
