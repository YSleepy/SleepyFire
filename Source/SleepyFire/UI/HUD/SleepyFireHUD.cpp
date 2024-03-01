// Copyright XSleepy. All Rights Reserved.


#include "UI/HUD/SleepyFireHUD.h"
#include "SleepyFireHUD.h"

UOverlayWidgetController* ASleepyFireHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void ASleepyFireHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("Check OverlayWidgetClass is NULL,Please fill out BP_SleepyFireHUD.OverlayWidgetClass"));
	checkf(OverlayWidgetControllerClass, TEXT("Check OverlayWidgetControllerClass is NULL,Please fill out BP_SleepyFireHUD.OverlayWidgetControllerClass"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USleepyFireUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);

	OverlayWidget->SetWidgetController(WidgetController);
	Widget->AddToViewport();
	WidgetController->BroadcastInitValues();
}
