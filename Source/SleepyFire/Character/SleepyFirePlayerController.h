// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SleepyFirePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SLEEPYFIRE_API ASleepyFirePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void PlayerCameraShake(TSubclassOf<UCameraShakeBase> CameraShake);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player UI")
		void ControllerInitUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player UI")
		void ControllerPlayFireUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Player UI")
		void ControllerUpdateAmmo(int32 SpareAmmo,int32 CurrentClipAmmo);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Player UI")
		void ControllerUpdateHealth(float NewHealth);
};
