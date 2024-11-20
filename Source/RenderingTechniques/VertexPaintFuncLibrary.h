// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VertexPaintFuncLibrary.generated.h"

class URuntimeVertexPaintingComponent;

/**
 * 
 */
UCLASS()
class RENDERINGTECHNIQUES_API UVertexPaintFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable)
	//static void PaintVerticesAtLocation(URuntimeVertexPaintingComponent* VertexPaintingComponent, FVector Location, float Radius, FColor ColorToPaint, bool bIgnoreVertexNormal);

	UFUNCTION(BlueprintCallable)
	static void PaintVerticesAtLocationV2(URuntimeVertexPaintingComponent* VertexPaintingComponent, FVector Location, float Radius, FColor ColorToPaint);

private:
};
