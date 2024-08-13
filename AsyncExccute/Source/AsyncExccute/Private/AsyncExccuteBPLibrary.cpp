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

	// ����CMD�����Ϣ
	Node->Process->OnOutput().BindLambda([Node](const FString& Output)
	{
		// ����������ͨ��Update�¼����ݳ�ȥ
		Node->Update.Broadcast(Output);  // ������Ը���ʵ�����󴫵ݲ�ͬ����Ϣ
	});


	// ����CMD���������¼�
	Node->Process->OnCompleted().BindLambda([Node](int32 ReturnCode)
	{
			if (ReturnCode == 0)
			{
				Node->Success.Broadcast("Success");  // ���ݳɹ��¼�
			}
			else
			{
				Node->Failed.Broadcast("Failed");  // ����ʧ���¼�
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




