// Copyright XSleepy. All Rights Reserved.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "OverlayWidgetController.h"

#include "AbilitySystem/AttributeSet/SleepyFireHealthSet.h"

void UOverlayWidgetController::BroadcastInitValues()
{
	const USleepyFireHealthSet* HealthSet = Cast<USleepyFireHealthSet>(AttributeSet);
	OnHealthChanged.Broadcast(HealthSet->GetHealth());
	//OnMaxHealthChanged.Broadcast(HealthSet->GetMaxHealth());
}


void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const USleepyFireHealthSet* HealthSet = Cast<USleepyFireHealthSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
				OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	//TODO: if Cast fail, Bind In Client, Server is not Bind .
	Cast<USleepyFireAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& TagContainer)
		{
			for (const FGameplayTag& Tag : TagContainer)
			{
				if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Message")))))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, Tag.ToString());
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					OnMessageWidgetRowDelegate.Broadcast(*Row);
				}

			}
		});
}
