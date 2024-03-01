// Fill out your copyright notice in the Description page of Project Settings.


#include "SleepyFirePlayerController.h"

void ASleepyFirePlayerController::PlayerCameraShake(TSubclassOf<UCameraShakeBase> CameraShake)
{
	if (CameraShake) {
		ClientStartCameraShake(CameraShake);
	}
}
