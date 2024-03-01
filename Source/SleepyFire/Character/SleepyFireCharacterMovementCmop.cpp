// Copyright XSleepy. All Rights Reserved.

#include "Character/SleepyFireCharacterMovementCmop.h"

#include "GameFramework/Character.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"




// Sets default values for this component's properties
USleepyFireCharacterMovementCmop::USleepyFireCharacterMovementCmop()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void USleepyFireCharacterMovementCmop::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USleepyFireCharacterMovementCmop::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USleepyFireCharacterMovementCmop::InitMovementComp(AController* NewController)
{
	if(NewController)//Server Or Client Autonomous Proxy
	{
		if (CompOwner == nullptr)
			CompOwner = Cast<ACharacter>(GetOwner());
		if (PlayerController == nullptr)
			PlayerController = Cast<APlayerController>(NewController);
	}
	//else//Client
	//{
	//	while (CompOwner == nullptr)
	//		CompOwner = Cast<ACharacter>(GetOwner());
	//	if(CompOwner->IsLocallyControlled())
	//	{
	//		while (PlayerController == nullptr)
	//		PlayerController = Cast<APlayerController>(CompOwner->GetController());
	//	}
	//}
	EnableControllerInput();
}

void USleepyFireCharacterMovementCmop::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);


	if (EnhancedInputComponent) {
		if (this->IA_Jump) {
			EnhancedInputComponent->BindAction(this->IA_Jump, ETriggerEvent::Triggered, this, &USleepyFireCharacterMovementCmop::X_Jump);
			EnhancedInputComponent->BindAction(this->IA_Jump, ETriggerEvent::Completed, this, &USleepyFireCharacterMovementCmop::X_JumpStop);
		}
		if (this->IA_Look) {
			EnhancedInputComponent->BindAction(this->IA_Look, ETriggerEvent::Triggered, this, &USleepyFireCharacterMovementCmop::X_Look);
		}
		if (this->IA_Move) {
			EnhancedInputComponent->BindAction(this->IA_Move, ETriggerEvent::Triggered, this, &USleepyFireCharacterMovementCmop::X_Move);
			EnhancedInputComponent->BindAction(this->IA_Move, ETriggerEvent::Completed, this, &USleepyFireCharacterMovementCmop::X_NormalSpeedWalk);
		}
		if (this->IA_SpringOrNormal) {
			EnhancedInputComponent->BindAction(this->IA_SpringOrNormal, ETriggerEvent::Started, this, &USleepyFireCharacterMovementCmop::X_SpringSpeedWalk);
			EnhancedInputComponent->BindAction(this->IA_SpringOrNormal, ETriggerEvent::Ongoing, this, &USleepyFireCharacterMovementCmop::X_SpringSpeedWalk);
			EnhancedInputComponent->BindAction(this->IA_SpringOrNormal, ETriggerEvent::Triggered, this, &USleepyFireCharacterMovementCmop::X_SpringSpeedWalk);
			EnhancedInputComponent->BindAction(this->IA_SpringOrNormal, ETriggerEvent::Canceled, this, &USleepyFireCharacterMovementCmop::X_NormalSpeedWalk);
		}
	}
}

void USleepyFireCharacterMovementCmop::EnableControllerInput()
{
	if (PlayerController) {
		CompOwner->EnableInput(PlayerController);
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (EnhancedInputSubsystem&&Default_Mapping) {
			EnhancedInputSubsystem->AddMappingContext(Default_Mapping, 1);//优先级
		}
	}
}

void USleepyFireCharacterMovementCmop::X_Move(const FInputActionValue& Value)
{
	const FVector2D XY = Value.Get<FVector2D>();
	if (PlayerController) {
		CompOwner->AddMovementInput(CompOwner->GetActorForwardVector(), XY.X);
		CompOwner->AddMovementInput(CompOwner->GetActorRightVector(), XY.Y);
	}
}

void USleepyFireCharacterMovementCmop::X_Look(const FInputActionValue& Value)
{
	const FVector2D XY = Value.Get<FVector2D>();
	if (PlayerController) {
		CompOwner->AddControllerYawInput(XY.X);
		CompOwner->AddControllerPitchInput(XY.Y);
	}
}

void USleepyFireCharacterMovementCmop::X_Jump(const FInputActionValue& Value)
{
	const bool IsJump = Value.Get<bool>();
	if (PlayerController) {
		if (IsJump)CompOwner->Jump();
	}

}

void USleepyFireCharacterMovementCmop::X_JumpStop(const FInputActionValue& Value)
{
	const bool IsJump = Value.Get<bool>();
	if (PlayerController) {
		if (!IsJump)CompOwner->StopJumping();
	}
}

void USleepyFireCharacterMovementCmop::X_NormalSpeedWalk(const FInputActionValue& Value)
{
	if (PlayerController) {
		CompOwner->GetCharacterMovement()->MaxWalkSpeed = 300;
		this->ServerNormalSpeedWalk();
	}
}

void USleepyFireCharacterMovementCmop::X_SpringSpeedWalk(const FInputActionValue& Value)
{
	if (PlayerController) {
		CompOwner->GetCharacterMovement()->MaxWalkSpeed = 600;
		this->ServerSpringSpeedWalk();
	}
}

void USleepyFireCharacterMovementCmop::ServerNormalSpeedWalk_Implementation()
{
	if (PlayerController) {
		CompOwner->GetCharacterMovement()->MaxWalkSpeed = 300;
	}
}

bool USleepyFireCharacterMovementCmop::ServerNormalSpeedWalk_Validate()
{
	return true;
}

void USleepyFireCharacterMovementCmop::ServerSpringSpeedWalk_Implementation()
{
	if (PlayerController) {
		CompOwner->GetCharacterMovement()->MaxWalkSpeed = 600;
	}
}

bool USleepyFireCharacterMovementCmop::ServerSpringSpeedWalk_Validate()
{
	return true;
}
