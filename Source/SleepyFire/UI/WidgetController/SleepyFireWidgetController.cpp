// Copyright XSleepy. All Rights Reserved.


#include "UI/WidgetController/SleepyFireWidgetController.h"
#include "SleepyFireWidgetController.h"

void USleepyFireWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void USleepyFireWidgetController::BroadcastInitValues()
{
}

void USleepyFireWidgetController::BindCallbacksToDependencies()
{
}
