// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "SleepyFireEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};
/*
 * Remove policy only applicable to Infinity.
 * For example, in Honor of Kings, removing the number of layers of the Wild Blade
 */
UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class SLEEPYFIRE_API ASleepyFireEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASleepyFireEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//apply gameplay effect to target
	UFUNCTION(BlueprintCallable)
		void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable)
		void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
		void OnEndOverlap(AActor* TargetActor);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect")
		TSubclassOf<UGameplayEffect>InstanceGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		EEffectApplicationPolicy InstanceEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		TSubclassOf<UGameplayEffect>DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		TSubclassOf<UGameplayEffect>InfinityGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		EEffectApplicationPolicy InfinityEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		EEffectRemovePolicy InfinityEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*>ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
		float ActorEffectLevel = 1.f;
};
