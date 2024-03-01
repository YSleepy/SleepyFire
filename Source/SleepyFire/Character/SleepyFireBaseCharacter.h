// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "SleepyFirePlayerController.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "Weapon/X_WeaponBaseServer.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "AbilitySystem/SleepyFireAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/SleepyFireAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "SleepyFireAttributeComponent.h"
#include "SleepyFireCharacterMovementCmop.h"

#include "SleepyFireBaseCharacter.generated.h"

UCLASS()
class SLEEPYFIRE_API ASleepyFireBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	#pragma region Component
	// Scene components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SleepyFireCharacter", meta = (AllowPrivateAccess="true"))
	UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFireCharacter", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ArmMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFireCharacter", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BodyMesh;

	// Actor components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFireCharacter", meta = (AllowPrivateAccess = "true"))
		USleepyFireAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="SleepyFireCharacter", meta = (AllowPrivateAccess = "true"))
		USleepyFireCharacterMovementCmop* SleepyFireCharacterMovementCmop;
	#pragma endregion

public:
	USkeletalMeshComponent* GetArmMesh()const { return this->ArmMesh; }
	USkeletalMeshComponent* GetBodyMesh()const { return this->BodyMesh; }
	USkeletalMeshComponent* GetPrimaryWeaponClientMesh()const { return this->PrimaryWeaponClient->WeaponMesh; }
private:


#pragma region Property

	UAnimInstance* ArmAnimBP;
	UAnimInstance* BodyAnimBP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SleepyFireWeapon", meta = (AllowPrivateAccess = "true"))
		AX_WeaponBaseServer* PrimaryWeaponServer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SleepyFireWeapon", meta = (AllowPrivateAccess = "true"))
		AX_WeaponBaseClient* PrimaryWeaponClient;
	
	UPROPERTY(BlueprintReadOnly, Category = "SleepyFireController", meta = (AllowPrivateAccess = "true"))
		ASleepyFirePlayerController* SleepyPlayerController;
	
#pragma endregion
public:
	// Sets default values for this character's properties
	ASleepyFireBaseCharacter();

	void BeginWithEquipment();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UFUNCTION()
	//	void BeginPlayInit();

private:
	#pragma region InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_Reload;
	void X_Fire(const FInputActionValue& Value);
	void X_StopFire(const FInputActionValue& Value);
	void X_Reload(const FInputActionValue& Value);
	#pragma endregion

	#pragma region NetWork_EquipWeapon
	UFUNCTION(Client, Reliable)
		void ClientEquipPrimaryWeapon();
		void ClientEquipPrimaryWeapon_Implementation();
	#pragma endregion

	#pragma region NetWork_Fire
	UFUNCTION(Client, Reliable)
		void ClientFirePrimaryWeapon();
		void ClientFirePrimaryWeapon_Implementation();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFirePrimaryWeapon(FVector Location, FRotator Rotation);
		void ServerFirePrimaryWeapon_Implementation(FVector Location, FRotator Rotation);
		bool ServerFirePrimaryWeapon_Validate(FVector Location, FRotator Rotation);
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiFirePrimaryWeapon();
		void MultiFirePrimaryWeapon_Implementation();
		bool MultiFirePrimaryWeapon_Validate();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void MultiSpawnCraterToTarget(FVector Location, FRotator Rotation);
		void MultiSpawnCraterToTarget_Implementation(FVector Location, FRotator Rotation);
		bool MultiSpawnCraterToTarget_Validate(FVector Location, FRotator Rotation);


	#pragma endregion

public:
	void ServerEquipPrimaryWeapon(AX_WeaponBaseServer* WeaponServer);
	void BuyWeapon(EWeaponType WeaponType);
	void FirePrimaryWeapon();
	void StopFirePrimaryWeapon();
	void DamagePlayer(AActor* DamageActor, FVector& HitFromDirection, FHitResult& HitActor, UPhysicalMaterial* HitPart);

	////被击中的actor应用伤害
	//UFUNCTION()
	//	void OnHitApplyDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);
	//
	// 通过 IAbilitySystemInterface 继承
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Init ability actor info for the server when character is controlled
	virtual void PossessedBy(AController* NewController) override;
	// Init ability actor info for the client when PlayerState is Copied
	virtual void OnRep_PlayerState() override;
protected:
	virtual void InitAbilityActorInfo(AController* NewController);
};
