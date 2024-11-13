#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Async/Async.h"

USTRUCT()
struct FPaintVertexTask
{
	GENERATED_BODY()
public:
	FPaintVertexTask();
	
	FPaintVertexTask(TFunction<void()> InTaskFunction, FString InTaskName)
	: TaskFunction(InTaskFunction), TaskName(InTaskName) {}

	void Execute()
	{
		if (TaskFunction)
		{
			TaskFunction();
		}
	}

	FString GetTaskName() const { return TaskName; }
	
private:
	TFunction<void()> TaskFunction;
	FString TaskName;
};

USTRUCT()
struct FRuntimePaintTaskQueue
{
public:

	void AddTask(TFunction<void()> TaskFunction, const FString& TaskName)
	{
		TaskQueue.Enqueue(FPaintVertexTask(TaskFunction, TaskName));
		TryExecuteNextTask();
	}

private:
	TQueue<FPaintVertexTask> TaskQueue;
	bool bIsTaskRunning = false;

	void TryExecuteNextTask()
	{
		if (bIsTaskRunning || TaskQueue.IsEmpty())
		{
			return;
		}

		FPaintVertexTask NextTask;
		if (TaskQueue.Dequeue(NextTask))
		{
			bIsTaskRunning = true;
			NotifyTaskStart(NextTask.GetTaskName());

			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, &NextTask]()
			{
				NextTask.Execute();
				NotifyTaskFinish(NextTask.GetTaskName());

				AsyncTask(ENamedThreads::GameThread, [this]()
				{
					bIsTaskRunning = false;
					TryExecuteNextTask();
				});
			});
		}
	}

	void NotifyTaskStart(const FString& TaskName)
	{
		UE_LOG(LogTemp, Log, TEXT("Task Started: %s"), *TaskName);
	}

	void NotifyTaskFinish(const FString& TaskName)
	{
		UE_LOG(LogTemp, Log, TEXT("Task Finished: %s"), *TaskName);
	}
};
