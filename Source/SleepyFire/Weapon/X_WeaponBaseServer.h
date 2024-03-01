// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "X_WeaponBaseClient.h"
#include <Components/SphereComponent.h>
#include "X_WeaponBaseServer.generated.h"

UENUM(BlueprintType)
enum class EWeaponType :uint8 {
	AK47	UMETA(DisplayName="AK47"),
	DesertEagle	UMETA(DisplayName="DesertEagle"),
	MP7		UMETA(DisplayName="MP7"),
	SVD		UMETA(DisplayName = "SVD"),
	M4A1	UMETA(DisplayName = "M4A1")
};

class ASleepyFireBaseCharacter;

UCLASS()
class SLEEPYFIRE_API AX_WeaponBaseServer : public AActor
{
	GENERATED_BODY()
public:
	#pragma region Component
	UPROPERTY(EditAnywhere)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollision;
	#pragma endregion

	#pragma region Effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AX_WeaponBaseClient> WeaponBaseClient;

	UPROPERTY(EditAnywhere)
		UParticleSystem* FireParticle;
	
	UPROPERTY(EditAnywhere)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
		UAnimMontage* FireBodyAnimMontage;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UAnimInstance> ABP_WeaponDriveBody;

	UPROPERTY(EditAnywhere)
		UMaterialInstance* AmmoCraterMaterail;
	#pragma endregion

	#pragma region Property

	UPROPERTY(EditAnywhere)
		ASleepyFireBaseCharacter* OwnerSleepy_BaseCharacter;

	UPROPERTY(EditAnywhere, Replicated)
		int32 CurrentClipAmmo;

	UPROPERTY(EditAnywhere)
		int32 MaxClipAmmo;

	UPROPERTY(EditAnywhere, Replicated)
		int32 SpareAmmo;

	UPROPERTY(EditAnywhere)
		int32 MaxSpareAmmo;

	UPROPERTY(EditAnywhere)
		int32 AmmoRange;

	UPROPERTY(EditAnywhere)
		float BaseDamage;
	#pragma endregion
public:	
	// Sets default values for this actor's properties
	AX_WeaponBaseServer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void WillEquipWeapon();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	#pragma region FireEffect
	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void PlayWeaponFireParticleServer();
		void PlayWeaponFireParticleServer_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void PlayWeaponFireSoundServer();
		void PlayWeaponFireSoundServer_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		void SpawnCraterToTarget(FVector Location, FRotator Rotation);
		void SpawnCraterToTarget_Implementation(FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Fire")
		void PlayWeaponFireEffectServer();
	#pragma endregion

	#pragma region FireLogic
	UFUNCTION(BlueprintCallable, Category = "Fire")
		void CurrentClipAmmoReduce();

	UFUNCTION(BlueprintCallable, Category = "Fire")
		bool WeaponFireLogicServer(FVector CameraLocation, FRotator CameraRotation, bool IsMoving, FHitResult& HitActorResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		bool CanWeaponFire();
		bool CanWeaponFire_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Fire")
		bool WeaponFireLineTrace(FVector CameraLocation,FRotator CameraRotation,bool IsMoving, FHitResult& HitActorResult);
		bool WeaponFireLineTrace_Implementation(FVector CameraLocation, FRotator CameraRotation, bool IsMoving, FHitResult& HitActorResult);
	
	#pragma endregion
};
