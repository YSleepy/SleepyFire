// Fill out your copyright notice in the Description page of Project Settings.

#include "SleepyFireBaseCharacter.h"
#include<Kismet/GameplayStatics.h>
#include<Net/UnrealNetwork.h>
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <PhysicalMaterials/PhysicalMaterial.h>

#include "Player/SleepyFirePlayerState.h"

// Sets default values
ASleepyFireBaseCharacter::ASleepyFireBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	#pragma region ComponentInit
	this->SleepyFireCharacterMovementCmop = CreateDefaultSubobject<USleepyFireCharacterMovementCmop>(TEXT("SleepyFireCharacterMovementCmop"));
	this->AttributeComponent = CreateDefaultSubobject<USleepyFireAttributeComponent>(TEXT("AttributeComponent"));
	this->AttributeComponent->SetIsReplicated(true);
	this->PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	if (this->PlayerCamera) {
		this->PlayerCamera->SetupAttachment(RootComponent);
		this->PlayerCamera->bUsePawnControlRotation = true;
	}
	this->ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	if (this->ArmMesh) {
		this->ArmMesh->SetupAttachment(this->PlayerCamera);
		this->ArmMesh->SetOnlyOwnerSee(true);
	}
	this->BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	if (this->BodyMesh) {
		this->BodyMesh->SetupAttachment(RootComponent);
		this->BodyMesh->SetOwnerNoSee(true);
		this->BodyMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//Only Query
		this->BodyMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	}
	#pragma endregion
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

void ASleepyFireBaseCharacter::BeginWithEquipment()
{
	if (HasAuthority()) {
		//UE_LOG(LogTemp, Log, TEXT("Hello"));
		BuyWeapon(EWeaponType::DesertEagle);
	}
}

// Called when the game starts or when spawned
void ASleepyFireBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//FLatentActionInfo LatentInfo;
	//LatentInfo.CallbackTarget = this;
	//LatentInfo.ExecutionFunction = "BeginPlayInit"; //function名
	//LatentInfo.Linkage = 1;
	//LatentInfo.UUID = 12345;
	//UKismetSystemLibrary::Delay(this, 3.0f, LatentInfo);
}

// Called every frame
void ASleepyFireBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ASleepyFireBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SleepyFireCharacterMovementCmop->SetupPlayerInputComponent(PlayerInputComponent);
	//if (APlayerController* playController = CastChecked<APlayerController>(GetController())) {
	//	EnableInput(playController);
	//	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playController->GetLocalPlayer());
	//	if (Default_Mapping) {
	//		EnhancedInputSubsystem->AddMappingContext(Default_Mapping,1);//优先级
	//	}
	//}
	//	if (this->IA_Fire) {
	//		EnInputComponent->BindAction(this->IA_Fire, ETriggerEvent::Triggered, this, &ASleepyFireBaseCharacter::X_Fire);
	//		EnInputComponent->BindAction(this->IA_Fire, ETriggerEvent::Completed, this, &ASleepyFireBaseCharacter::X_StopFire);
	//	}
	//	if (this->IA_Reload) {
	//		EnInputComponent->BindAction(this->IA_Reload, ETriggerEvent::Started, this, &ASleepyFireBaseCharacter::X_Reload);
	//	}
	
}

//void ASleepyFireBaseCharacter::BeginPlayInit()
//{
//	this->ArmAnimBP = ArmMesh->GetAnimInstance();
//	this->BodyAnimBP = BodyMesh->GetAnimInstance();
//	//在客户端中有些Pawn是没有控制器的，仅模拟的，这里如果使用CastChecked将会奔溃
//
//	this->SleepyPlayerController = Cast<ASleepyFirePlayerController>(GetController());
//	if (this->SleepyPlayerController && this->SleepyPlayerController->IsLocalController()) {
//		this->SleepyPlayerController->ControllerInitUI();
//		this->SleepyPlayerController->ControllerUpdateHealth(this->Health);
//	}
//	BeginWithEquipment();
//	if (this->PrimaryWeaponServer && this->SleepyPlayerController && this->SleepyPlayerController ->IsLocalPlayerController()) {
//		this->SleepyPlayerController->ControllerUpdateAmmo(this->PrimaryWeaponServer->SpareAmmo, this->PrimaryWeaponServer->CurrentClipAmmo);
//	}
//	OnTakePointDamage.AddDynamic(this, &ASleepyFireBaseCharacter::OnHitApplyDamage);
//	
//}



void ASleepyFireBaseCharacter::X_Fire(const FInputActionValue& Value)
{
	if (this->PrimaryWeaponServer) {
		switch (this->PrimaryWeaponServer->WeaponType)
		{
		case EWeaponType::AK47:
			break;
		case EWeaponType::DesertEagle: {
			this->FirePrimaryWeapon();
		}
			break;
		case EWeaponType::MP7:
			break;
		case EWeaponType::SVD:
			break;
		case EWeaponType::M4A1:
			break;
		default:
			break;
		}
	}
}

void ASleepyFireBaseCharacter::X_StopFire(const FInputActionValue& Value)
{
	if (this->PrimaryWeaponServer) {
		switch (this->PrimaryWeaponServer->WeaponType)
		{
		case EWeaponType::AK47:
			break;
		case EWeaponType::DesertEagle:
			break;
		case EWeaponType::MP7:
			break;
		case EWeaponType::SVD:
			break;
		case EWeaponType::M4A1:
			break;
		default:
			break;
		}
	}
}

void ASleepyFireBaseCharacter::X_Reload(const FInputActionValue& Value)
{

}


void ASleepyFireBaseCharacter::ClientEquipPrimaryWeapon_Implementation()
{
	if (this->PrimaryWeaponClient) {

	}
	else {
	if (this->PrimaryWeaponServer) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		this->PrimaryWeaponClient = GetWorld()->SpawnActor<AX_WeaponBaseClient>(this->PrimaryWeaponServer->WeaponBaseClient,GetActorTransform(), SpawnInfo);
		if (PrimaryWeaponClient) {
			PrimaryWeaponClient->K2_AttachToComponent(ArmMesh, "WeaponSocket",
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget, true);
		}
	}
	}

}

void ASleepyFireBaseCharacter::ClientFirePrimaryWeapon_Implementation()
{
	if (this->PrimaryWeaponClient) {
		//客户端武器播放效果
		this->PrimaryWeaponClient->PlayWeaponFireEffectClient();
		//客户端Arm 动画
		this->ArmAnimBP->Montage_Play(this->PrimaryWeaponClient->FireArmAnimMontage);
		//客户端摄像机晃动
		this->SleepyPlayerController->PlayerCameraShake(this->PrimaryWeaponClient->FireCameraShakeClass);
		//客户端UI动画
		this->SleepyPlayerController->ControllerPlayFireUI();
		//客户端更新UI Ammo
		this->SleepyPlayerController->ControllerUpdateAmmo(this->PrimaryWeaponServer->SpareAmmo, HasAuthority()? this->PrimaryWeaponServer->CurrentClipAmmo:this->PrimaryWeaponServer->CurrentClipAmmo - 1);
	}
	
}

void ASleepyFireBaseCharacter::ServerFirePrimaryWeapon_Implementation(FVector Location, FRotator Rotation)
{
	FHitResult HitActorResult;
	bool HitOk = this->PrimaryWeaponServer->WeaponFireLogicServer(Location, Rotation,false,HitActorResult);
	//多播必须在服务端调用才有用
	this->MultiFirePrimaryWeapon();//通用多播
	if (HitOk) {

		ASleepyFireBaseCharacter* Hit = Cast<ASleepyFireBaseCharacter>(HitActorResult.GetActor());

		if (Hit) {
			DamagePlayer(HitActorResult.GetActor(),Location,HitActorResult,HitActorResult.PhysMaterial.Get());
		}
		else {
			FRotator RectifyRotation = UKismetMathLibrary::MakeRotFromX(HitActorResult.Normal);
			this->MultiSpawnCraterToTarget(HitActorResult.Location, RectifyRotation);
		}
	}
}

bool ASleepyFireBaseCharacter::ServerFirePrimaryWeapon_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void ASleepyFireBaseCharacter::MultiFirePrimaryWeapon_Implementation()
{
	//多播，但是自己的客户端本地pawn不调用
	//GetOwner()
	if (GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0)&& this->PrimaryWeaponServer) {
		this->PrimaryWeaponServer->PlayWeaponFireEffectServer();
		this->BodyAnimBP->Montage_Play(this->PrimaryWeaponServer->FireBodyAnimMontage);
	}
	
}

bool ASleepyFireBaseCharacter::MultiFirePrimaryWeapon_Validate()
{
	return true;
}

void ASleepyFireBaseCharacter::MultiSpawnCraterToTarget_Implementation(FVector Location, FRotator Rotation)
{
	if(this->PrimaryWeaponServer)
		PrimaryWeaponServer->SpawnCraterToTarget(Location, Rotation);
}

bool ASleepyFireBaseCharacter::MultiSpawnCraterToTarget_Validate(FVector Location, FRotator Rotation)
{
	return true;
}

void ASleepyFireBaseCharacter::InitAbilityActorInfo(AController* NewController)
{
	AttributeComponent->InitAbilityActorInfo(NewController);
}

void ASleepyFireBaseCharacter::ServerEquipPrimaryWeapon(AX_WeaponBaseServer* WeaponServer)
{
	if (this->PrimaryWeaponServer) {

	}
	else {
		PrimaryWeaponServer = WeaponServer;
		PrimaryWeaponServer->SetOwner(this);
		PrimaryWeaponServer->K2_AttachToComponent(BodyMesh,"Hand_r",
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,true);
		ClientEquipPrimaryWeapon();

		if (this->SleepyPlayerController) {
			if (this->PrimaryWeaponServer) {
				this->SleepyPlayerController->ControllerUpdateAmmo(this->PrimaryWeaponServer->SpareAmmo, this->PrimaryWeaponServer->CurrentClipAmmo);
				this->PrimaryWeaponServer->OwnerSleepy_BaseCharacter = this;
				UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("装备：%x"), this));
			}
		}
	}
}

void ASleepyFireBaseCharacter::BuyWeapon(EWeaponType WeaponType)
{
	if (HasAuthority()) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		switch (WeaponType)
		{
		case EWeaponType::AK47:
			break;
		case EWeaponType::DesertEagle: {
			UClass* ClassWeaponBaseServer = StaticLoadClass(AX_WeaponBaseServer::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Weapon/DesertEagle/BP_WeaponDesertEagleServer.BP_WeaponDesertEagleServer_C'"));
			AX_WeaponBaseServer* WeaponServer = GetWorld()->SpawnActor<AX_WeaponBaseServer>(ClassWeaponBaseServer, GetActorTransform(), SpawnInfo);
			WeaponServer->WillEquipWeapon();
			this->ServerEquipPrimaryWeapon(WeaponServer);
		}
			break;
		case EWeaponType::MP7:
			break;
		case EWeaponType::SVD:
			break;
		case EWeaponType::M4A1:
			break;
		default:
			break;
		}
	}

}

void ASleepyFireBaseCharacter::FirePrimaryWeapon()
{
	if (this->PrimaryWeaponServer->CanWeaponFire()&& this->SleepyPlayerController) {
		//服务端重要逻辑，服务端需要客户端的数据
		this->ServerFirePrimaryWeapon(this->PlayerCamera->GetComponentLocation(), this->PlayerCamera->GetComponentRotation());
		//客户端视觉效果,服务器不做,由于网络延迟客户端要模拟一些数据的变化
		this->ClientFirePrimaryWeapon();
		
	}
}

void ASleepyFireBaseCharacter::StopFirePrimaryWeapon()
{
}

void ASleepyFireBaseCharacter::DamagePlayer(AActor* DamageActor, FVector& HitFromDirection, FHitResult& HitActor,UPhysicalMaterial* HitPart)
{
	float FinallyDamage = this->PrimaryWeaponServer->BaseDamage;
	//EPhysicalSurface in project setting。
	switch (HitPart->SurfaceType)
	{
	case EPhysicalSurface::SurfaceType1: {
		FinallyDamage *= 3;
	}break;
	case EPhysicalSurface::SurfaceType2: {
		FinallyDamage *= 1;
	}break;
	case EPhysicalSurface::SurfaceType3: {
		FinallyDamage *= 0.8;
	}break;
	case EPhysicalSurface::SurfaceType4: {
		FinallyDamage *= 0.7;
	}break;
	case EPhysicalSurface::SurfaceType5: {
		FinallyDamage *= 0.6;
	}break;
	}

	UGameplayStatics::ApplyPointDamage(DamageActor, FinallyDamage, HitFromDirection, HitActor, GetController(), this, UDamageType::StaticClass());
}

//void ASleepyFireBaseCharacter::OnRep_Health()const
//{
//	if(this->SleepyPlayerController)
//	this->SleepyPlayerController->ControllerUpdateHealth(this->Health);
//}

//void ASleepyFireBaseCharacter::OnHitApplyDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
//{
//	if (this->Health > 0) {
//		const float Temp_Health = this->Health - Damage;
//		if (Temp_Health <= 0) {
//			this->Health = 0;
//		}
//		else {
//		this->Health -= Damage;	
//		}
//		UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Health:%f"), Health));
//
//	}
//}

UAbilitySystemComponent* ASleepyFireBaseCharacter::GetAbilitySystemComponent() const
{
	return AttributeComponent->GetAbilitySystemComponent();
}


void ASleepyFireBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("PossessedBy is Call %p"), NewController);
	SleepyFireCharacterMovementCmop->InitMovementComp(NewController);
}

void ASleepyFireBaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerState is Call"));
	SleepyFireCharacterMovementCmop->InitMovementComp(GetController());
}

