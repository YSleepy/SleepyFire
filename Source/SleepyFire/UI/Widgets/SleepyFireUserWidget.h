// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/SleepyFireWidgetController.h"
#include "SleepyFireUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API USleepyFireUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void SetWidgetController(UObject* InWidgetController);
public:
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<UObject>WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
		void WidgetControllerSet();
};
