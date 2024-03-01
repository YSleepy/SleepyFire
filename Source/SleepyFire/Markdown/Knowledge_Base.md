# UE源码

针对UE4源码版，项目级别的清理步骤：

（1）删除项目根目录中：Binaries、Build、DerivedDataCache 和 Intermediate 文件夹，以及 sln文件。

（2）通过 .uproject 重新生成Visual Studio项目文件，此步骤会重新生成 sln文件 和 Intermediate 文件夹。

（3）打开 sln 项目解决方案，将自建项目【设为启动项目】，并构建项目（特别注意不能重新构建项目），会重新生成 Binaries 和 Build 文件夹。

（4）运行项目，打开编辑器的同时会重新生成 DerivedDataCache 文件夹。

（5）始终记得：

在源码版UE4的解决方案中，不要使用Visual Studio的【清理】和【重新生成】功能！

# Camera
实现摄像机旋转
```C++
AddControllerPitchInput(XY.Y);//给控制器添加选择

this->PalyerCamera->bUsePawnControlRotation = true;//使用pawn控制选择（实际上是使用控制器旋转），否则将跟随父类旋转

void UCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView){//摄像机组件每一帧都会调用
	//...
	if (bUsePawnControlRotation)
	{
		const APawn* OwningPawn = Cast<APawn>(GetOwner());
		const AController* OwningController = OwningPawn ? OwningPawn->GetController() : nullptr;
		if (OwningController && OwningController->IsLocalPlayerController())
		{
			const FRotator PawnViewRotation = OwningPawn->GetViewRotation();
			if (!PawnViewRotation.Equals(GetComponentRotation()))
			{
				SetWorldRotation(PawnViewRotation);//并且是相对世界的选择而非父类
			}
		}
	}
	//...
}

```
# Movement

## Jump
```C++
Jump();//调用后

void ACharacter::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
}

void ACharacter::CheckJumpInput(float DeltaTime)//每帧调用
{
	JumpCurrentCountPreJump = JumpCurrentCount;

	if (CharacterMovement)
	{
		if (bPressedJump)//jump按下
		{
			// If this is the first jump and we're already falling,
			// then increment the JumpCount to compensate.
			const bool bFirstJump = JumpCurrentCount == 0;
			if (bFirstJump && CharacterMovement->IsFalling())
			{
				JumpCurrentCount++;
			}

			const bool bDidJump = CanJump() && CharacterMovement->DoJump(bClientUpdating);
			if (bDidJump)
			{
				// Transition from not (actively) jumping to jumping.
				if (!bWasJumping)
				{
					JumpCurrentCount++;
					JumpForceTimeRemaining = GetJumpMaxHoldTime();
					OnJumped();
				}
			}

			bWasJumping = bDidJump;
		}
	}
}

```
# 反射机制

使用反射机制得到属性集

```C++
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
```
