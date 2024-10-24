// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAsyncTaskFinished);

UCLASS()
class RENDERINGTECHNIQUES_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestActor();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void PaintSMVertices();

	UFUNCTION(BlueprintCallable)
	void PaintSMVerticesAtLocation(FVector Location, FColor Color, float Radius);
	
	UFUNCTION(BlueprintCallable)
    void PaintVerticesAtLocation(FVector HitLocation, FColor TargetBaseColor, float PaintLerpProgress, float BrushSize);

	UFUNCTION(BlueprintCallable)
	void GetVertexColorsinArea(FVector HitLocation, float Radius);
    	
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChangeAtRuntime = false;

	TArray<int32> VerticesIds;
	
	TArray<FColor> ExistingColors;

	FOnAsyncTaskFinished OnAsyncTaskFinished;

	UFUNCTION()
	void OnAsyncTaskFinishedFunc();

	bool bHasWaited = false;

private:
	UStaticMesh* DuplicateStaticMesh(UStaticMesh* OriginalMesh);
};
