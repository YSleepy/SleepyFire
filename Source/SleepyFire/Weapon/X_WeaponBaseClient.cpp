// Fill out your copyright notice in the Description page of Project Settings.


#include "X_WeaponBaseClient.h"
#include<Kismet/GameplayStatics.h>
// Sets default values
AX_WeaponBaseClient::AX_WeaponBaseClient()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(this->WeaponMesh);
	this->WeaponMesh->SetOnlyOwnerSee(true);
	this->WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AX_WeaponBaseClient::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AX_WeaponBaseClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AX_WeaponBaseClient::PlayWeaponFireAnimClient_Implementation()
{
	if(this->FireWeaponAnimSequence)
	this->WeaponMesh->PlayAnimation(this->FireWeaponAnimSequence,false);
}

void AX_WeaponBaseClient::PlayWeaponFireSoundClient_Implementation()
{
	if(this->FireSound)
	UGameplayStatics::PlaySound2D(GetWorld(), this->FireSound);
}

void AX_WeaponBaseClient::PlayWeaponFireParticleClient_Implementation()
{
	if (this->FireParticle)
		UGameplayStatics::SpawnEmitterAttached(this->FireParticle, this->WeaponMesh, TEXT("Fire_FX_Slot"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
}

void AX_WeaponBaseClient::PlayWeaponFireEffectClient()
{
	PlayWeaponFireAnimClient();
	PlayWeaponFireSoundClient();
	PlayWeaponFireParticleClient();
}

