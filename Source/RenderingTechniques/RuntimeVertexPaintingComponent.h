// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RuntimeVertexPaintingComponent.generated.h"

class URuntimeVertexPaintingComponent;
USTRUCT(Blueprintable, BlueprintType)
struct FRVPVerticesStructure
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<URuntimeVertexPaintingComponent> VertexPaintingComponent;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	FStaticMeshLODResources* LODModel;
	
	FStaticMeshComponentLODInfo* LODInfo;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RENDERINGTECHNIQUES_API URuntimeVertexPaintingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	void InitializeVertexStructure();
	
	UPROPERTY()
	FRVPVerticesStructure VerticesStructure;
	
	TArray<FColor> Colors;
private:

	void InitializeColourData();
};
