#pragma once
#include "LoadingProcessInterface.h"
#include "UObject/Object.h"

#include "LoadingProcessTask.generated.h"

struct FFrame;

UCLASS(BlueprintType)
class LOADINGSCREEN_API ULoadingProcessTask : public UObject, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	//create task (WorldContext = "WorldContextObject") 
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
		static ULoadingProcessTask* CreateLoadingScreenProcessTask(UObject* WorldContextObject, const FString& ShowLoadingScreenReason);

public:
	ULoadingProcessTask() { }

	UFUNCTION(BlueprintCallable)
		void Unregister();

	UFUNCTION(BlueprintCallable)
		void SetShowLoadingScreenReason(const FString& InReason);

	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	FString Reason;
};
