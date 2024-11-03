// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VertexPaintFuncLibrary.generated.h"

class URuntimeVertexPaintingComponent;

USTRUCT()
struct FRVPVertexStructure
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<URuntimeVertexPaintingComponent> VertexPaintingComponent;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	FStaticMeshLODResources& LODModel;
	
	TObjectPtr<FStaticMeshComponentLODInfo> LODInfo;

};

/**
 * 
 */
UCLASS()
class RENDERINGTECHNIQUES_API UVertexPaintFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void PaintVerticesAtLocation(URuntimeVertexPaintingComponent* VertexPaintingComponent, FVector Location, float Radius, FColor ColorToPaint, bool bIgnoreVertexNormal);


private:
	void InitializeTask();
};
