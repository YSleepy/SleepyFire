

// Fill out your copyright notice in the Description page of Project Settings.

#include "X_WeaponBaseServer.h"
#include "Character/SleepyFireBaseCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/DecalComponent.h>

// Sets default values
AX_WeaponBaseServer::AX_WeaponBaseServer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	#pragma region ComponentInit
	this->WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	this->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	this->WeaponMesh->SetOwnerNoSee(true);
	SetRootComponent(this->WeaponMesh);

	this->SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	this->SphereCollision->SetupAttachment(RootComponent);

	this->SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	this->WeaponMesh->SetEnableGravity(true);
	this->WeaponMesh->SetSimulatePhysics(true);
	#pragma endregion

	this->SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AX_WeaponBaseServer::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AX_WeaponBaseServer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AX_WeaponBaseServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AX_WeaponBaseServer::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASleepyFireBaseCharacter* BaseCharacter = Cast<ASleepyFireBaseCharacter>(OtherActor);
	if (BaseCharacter) {
		WillEquipWeapon();
		BaseCharacter->ServerEquipPrimaryWeapon(this);
	}
}

void AX_WeaponBaseServer::WillEquipWeapon()
{
	this->WeaponMesh->SetEnableGravity(false);
	this->WeaponMesh->SetSimulatePhysics(false);
	this->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AX_WeaponBaseServer::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AX_WeaponBaseServer, CurrentClipAmmo);
	DOREPLIFETIME(AX_WeaponBaseServer, SpareAmmo);
}


void AX_WeaponBaseServer::PlayWeaponFireParticleServer_Implementation()
{
	if (this->FireParticle)
		UGameplayStatics::SpawnEmitterAttached(this->FireParticle, this->WeaponMesh, TEXT("Fire_FX_Slot"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void AX_WeaponBaseServer::PlayWeaponFireSoundServer_Implementation()
{
	if(this->FireSound)
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->FireSound,this->GetActorLocation());
}

void AX_WeaponBaseServer::SpawnCraterToTarget_Implementation(FVector Location, FRotator Rotation)
{
	UDecalComponent* AmmoCrater = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), this->AmmoCraterMaterail, FVector(8, 8, 8), Location, Rotation, 10);
	if(AmmoCrater)
	AmmoCrater->SetFadeScreenSize(0.001);
}

void AX_WeaponBaseServer::PlayWeaponFireEffectServer()
{
	PlayWeaponFireParticleServer();
	PlayWeaponFireSoundServer();
}

void AX_WeaponBaseServer::CurrentClipAmmoReduce()
{
	this->CurrentClipAmmo--;
}

bool AX_WeaponBaseServer::WeaponFireLogicServer(FVector CameraLocation, FRotator CameraRotation, bool IsMoving, FHitResult& HitActorResult)
{
	CurrentClipAmmoReduce();
	//UKismetSystemLibrary::PrintString(GetWorld(),FString::Printf(TEXT("CurrentAmmo:%d"), this->CurrentClipAmmo));
	return this->WeaponFireLineTrace(CameraLocation, CameraRotation, IsMoving, HitActorResult);
}

bool AX_WeaponBaseServer::CanWeaponFire_Implementation()
{
	return CurrentClipAmmo > 0;
}

bool AX_WeaponBaseServer::WeaponFireLineTrace_Implementation(FVector CameraLocation, FRotator CameraRotation, bool IsMoving, FHitResult& HitActorResult)
{
	FVector CameraForwardVector = UKismetMathLibrary::GetForwardVector(CameraRotation);
	FVector CameraEndLocation;
	TArray<AActor*>IgnoreActorArray;
	IgnoreActorArray.Add(this);
	IgnoreActorArray.Add(this->OwnerSleepy_BaseCharacter);
	if (IsMoving) {

	}
	else {
		CameraEndLocation = CameraLocation + CameraForwardVector * this->AmmoRange;

	}
	bool HitOk = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		CameraLocation,
		CameraEndLocation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActorArray,
		EDrawDebugTrace::ForDuration,
		HitActorResult,
		true);
	return HitOk;
}

