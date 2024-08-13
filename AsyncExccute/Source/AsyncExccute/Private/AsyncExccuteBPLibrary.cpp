// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncExccuteBPLibrary.h"
#include "AsyncExccute.h"


UAsyncExccuteBPLibrary::UAsyncExccuteBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

UAsyncExccuteBPLibrary* UAsyncExccuteBPLibrary::ExecuteCommand( FString Command, FString Parameters, bool InHidden, bool InCreatePipes)
{
	UAsyncExccuteBPLibrary* Node = NewObject<UAsyncExccuteBPLibrary>();

	Node->Process = MakeShareable(new FMonitoredProcess(Command,Parameters, InHidden, InCreatePipes));

	// 捕获CMD输出信息
	Node->Process->OnOutput().BindLambda([Node](const FString& Output)
	{
		// 将捕获的输出通过Update事件传递出去
		Node->Update.Broadcast(Output);  // 这里可以根据实际需求传递不同的信息
	});


	// 处理CMD命令的完成事件
	Node->Process->OnCompleted().BindLambda([Node](int32 ReturnCode)
	{
			if (ReturnCode == 0)
			{
				Node->Success.Broadcast("Success");  // 传递成功事件
			}
			else
			{
				Node->Failed.Broadcast("Failed");  // 传递失败事件
			}
	});


	if (Node->Process->Launch())
	{
		UE_LOG(LogTemp, Log, TEXT("Process launched successfully."));
		Node->bIsRunning = true;
		while (Node->Process.IsValid() && !Node->Process->Update())
		{
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch process."));
	}
	return Node;
}




