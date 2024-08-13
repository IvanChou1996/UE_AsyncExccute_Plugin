// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Misc/MonitoredProcess.h"
#include "AsyncExccuteBPLibrary.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncNodeResult, FString, Result);



UCLASS()
class UAsyncExccuteBPLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FAsyncNodeResult Success;

	UPROPERTY(BlueprintAssignable)
	FAsyncNodeResult Failed;

	UPROPERTY(BlueprintAssignable)
	FAsyncNodeResult Update;

private:
	TSharedPtr<FMonitoredProcess> Process;
	bool bIsRunning;

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContestObject"))
	static UAsyncExccuteBPLibrary* ExecuteCommand( FString Command, FString Parameters,bool InHidden, bool InCreatePipes);


};


