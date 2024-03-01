// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "SleepyFireCharacterMovementCmop.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLEEPYFIRE_API USleepyFireCharacterMovementCmop : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USleepyFireCharacterMovementCmop();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitMovementComp(AController* NewController);
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
private:
	UFUNCTION()
	void EnableControllerInput();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFire|MovementComp", meta = (AllowPrivateAccess = "true"))
		ACharacter* CompOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFire|MovementComp", meta = (AllowPrivateAccess = "true"))
		APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFire|MovementComp", meta = (AllowPrivateAccess = "true"))
		UEnhancedInputComponent* EnhancedInputComponent;
#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SleepyFire|Input", meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* Default_Mapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SleepyFire|Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SleepyFire|Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SleepyFire|Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SleepyFire|Input", meta = (AllowPrivateAccess = "true"))
		class UInputAction* IA_SpringOrNormal;


#pragma endregion

#pragma region InputAction
	void X_Move(const FInputActionValue& Value);
	void X_Look(const FInputActionValue& Value);
	void X_Jump(const FInputActionValue& Value);
	void X_JumpStop(const FInputActionValue& Value);
	void X_NormalSpeedWalk(const FInputActionValue& Value);
	void X_SpringSpeedWalk(const FInputActionValue& Value);
#pragma endregion

#pragma region NetWork_WalkOrRun
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerNormalSpeedWalk();
	void ServerNormalSpeedWalk_Implementation();
	bool ServerNormalSpeedWalk_Validate();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpringSpeedWalk();
	void ServerSpringSpeedWalk_Implementation();
	bool ServerSpringSpeedWalk_Validate();
#pragma endregion
};
