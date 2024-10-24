// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

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
    	
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChangeAtRuntime = false;

private:
	UStaticMesh* DuplicateStaticMesh(UStaticMesh* OriginalMesh);
};
