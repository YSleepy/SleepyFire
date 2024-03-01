// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/SleepyFireUserWidget.h"
#include "SleepyFireHUD.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API ASleepyFireHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TObjectPtr<USleepyFireUserWidget>OverlayWidget;

	UFUNCTION()
		UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	//TODO: Decide where to call InitOverlay
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	//virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<USleepyFireUserWidget>OverlayWidgetClass;
	UPROPERTY()
		TObjectPtr<UOverlayWidgetController>OverlayWidgetController;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UOverlayWidgetController>OverlayWidgetControllerClass;
};
