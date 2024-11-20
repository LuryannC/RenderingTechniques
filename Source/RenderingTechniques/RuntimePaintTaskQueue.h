#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Async/Async.h"

struct FPaintVertexTask
{
public:
	TFunction<void()> TaskFunction;
	FString TaskName;
	ENamedThreads::Type ThreadType;
	EAsyncExecution AsyncExecution;

	FPaintVertexTask() = default;
	FPaintVertexTask(const TFunction<void()>& InTaskFunction, const FString& InTaskName, ENamedThreads::Type InThreadType)
	: TaskFunction(InTaskFunction), TaskName(InTaskName), ThreadType(InThreadType), AsyncExecution() {}

	FPaintVertexTask(const TFunction<void()>& InTaskFunction, const FString& InTaskName, EAsyncExecution InExecutionType = EAsyncExecution::ThreadPool)
	: TaskFunction(InTaskFunction), TaskName(InTaskName), ThreadType(), AsyncExecution(InExecutionType) {}

	bool IsTaskValid() const
	{
		return TaskFunction != nullptr && !TaskName.IsEmpty();
	}

	void ExecuteTask() const
	{
		if (IsTaskValid())
		{
			TaskFunction();
		}
	}
};

struct FRuntimePaintTaskQueue
{
	TArray<FPaintVertexTask> Tasks;
	TQueue<FPaintVertexTask, EQueueMode::Spsc> TaskQueue;

	void RegisterTask(const TFunction<void()>& InTaskFunction, const FString& InTaskName, ENamedThreads::Type InThreadType)
	{
		const FPaintVertexTask Task(InTaskFunction, InTaskName, InThreadType);
		Tasks.Add(Task);
		TaskQueue.Enqueue(Task);
		UE_LOG(LogTemp, Log, TEXT("Task registered: %s"), *InTaskName);
	}
	
	void RegisterTask(const TFunction<void()>& InTaskFunction, const FString& InTaskName, EAsyncExecution InAsyncExecution)
	{
		const FPaintVertexTask Task(InTaskFunction, InTaskName, InAsyncExecution);
		Tasks.Add(Task);
		TaskQueue.Enqueue(Task);
		UE_LOG(LogTemp, Log, TEXT("Task registered: %s"), *InTaskName);
	}

	void RunTasks()
	{
		if(TaskQueue.IsEmpty()) return;
		
		FPaintVertexTask NextTask;
		if (TaskQueue.Dequeue(NextTask))
		{
			if (NextTask.ThreadType)
			{
				AsyncTask(NextTask.ThreadType, [this, NextTask]()
				{
					NextTask.ExecuteTask();
				});
			}
			else
			{
				Async(NextTask.AsyncExecution, [this, NextTask]()
				{
					NextTask.ExecuteTask();
				});
			}
			RunTasks();
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
