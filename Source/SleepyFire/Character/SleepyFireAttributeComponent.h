// Copyright XSleepy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/SleepyFireHealthSet.h"
#include "AbilitySystem/AttributeSet/SleepyFireManaSet.h"
#include "Components/ActorComponent.h"
#include"GameFramework/Character.h"
#include "UObject/UnrealTypePrivate.h"
#include "SleepyFireAttributeComponent.generated.h"


UENUM(BlueprintType)
enum class ESleepyFireDeathState :uint8 {
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLEEPYFIRE_API USleepyFireAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USleepyFireAttributeComponent();

	void InitAttributeSets();

	UFUNCTION(BlueprintPure, Category = "SleepyFire|Attribute")
		static USleepyFireAttributeComponent* FindAttributeComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<USleepyFireAttributeComponent>() : nullptr); }

	template<typename AttributeSetClass>
	AttributeSetClass* FindAttributeSet();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Ability")
		USleepyFireAbilitySystemComponent* GetAbilitySystemComponent() const;

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Health")
		float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Health")
		float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Health")
		float GetHealthNormalized() const;

	// Returns the current Mana value.
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Mana")
		float GetMana() const;

	// Returns the current maximum Mana value.
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Mana")
		float GetMaxMana() const;

	// Returns the current Mana in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "SleepyFire|Attribute|Mana")
		float GetManaNormalized() const;

	void InitAbilityActorInfo(AController* NewController);
	//virtual void PostInitProperties() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFire|AttributeComp", meta = (AllowPrivateAccess = "true"))
	ACharacter* CompOwner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SleepyFire|AttributeComp", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;
	UPROPERTY()
		TObjectPtr <USleepyFireAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr<USleepyFireHealthSet>HealthSet;
	UPROPERTY()
		TObjectPtr<USleepyFireManaSet>ManaSet;

};

template<typename AttributeSetClass>
AttributeSetClass* USleepyFireAttributeComponent::FindAttributeSet()
{
	const UClass* ThisClass = USleepyFireAttributeComponent::StaticClass();
	const UClass* AttributeClass = AttributeSetClass::StaticClass();

	TFieldIterator<FProperty> PropertyIt(ThisClass);
	for (; PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		TObjectPtr<AttributeSetClass>* Value = Property->ContainerPtrToValuePtr<TObjectPtr<AttributeSetClass>>(this);
		AttributeSetClass* Result = Cast<AttributeSetClass>(*Value);
		if(Result)
		{
		UE_LOG(LogTemp, Warning,TEXT("%s"), *FString::Printf(TEXT("%s:%p||%p"), *Property->GetFName().ToString(),this->HealthSet, *Value));
		return Result;
		}
		
	}
	return nullptr;
}
