#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "LoadingProcessInterface.generated.h"


UINTERFACE(BlueprintType)
class LOADINGSCREEN_API ULoadingProcessInterface : public UInterface
{
	GENERATED_BODY()
};

class LOADINGSCREEN_API ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	// Checks to see if this object implements the interface, and if so asks whether or not we should
	// be currently showing a loading screen
	static bool ShouldShowLoadingScreen(UObject* TestObject);

	virtual bool ShouldShowLoadingScreen(FString& OutReason) const
	{
		return false;
	}
};
