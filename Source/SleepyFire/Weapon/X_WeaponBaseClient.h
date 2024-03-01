// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "X_WeaponBaseClient.generated.h"

UCLASS()
class SLEEPYFIRE_API AX_WeaponBaseClient : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* FireArmAnimMontage;

	UPROPERTY(EditAnywhere)
		UAnimMontage* ReloadArmAnimMontage;

	UPROPERTY(EditAnywhere)
		UAnimSequence* FireWeaponAnimSequence;

	UPROPERTY(EditAnywhere)
		UAnimSequence* ReloadWeaponAnimSequence;

	UPROPERTY(EditAnywhere)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
		UParticleSystem* FireParticle;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShakeBase> FireCameraShakeClass;
public:	
	// Sets default values for this actor's properties
	AX_WeaponBaseClient();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void PlayWeaponFireAnimClient();
		void PlayWeaponFireAnimClient_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void PlayWeaponFireSoundClient();
		void PlayWeaponFireSoundClient_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void PlayWeaponFireParticleClient();
		void PlayWeaponFireParticleClient_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Fire")
		void PlayWeaponFireEffectClient();
};
