// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RuntimeVertexPaintingComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RENDERINGTECHNIQUES_API URuntimeVertexPaintingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URuntimeVertexPaintingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UStaticMeshComponent* GetStaticMeshComponent();
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	
	TArray<FColor> ExistingColors;
private:
			
	TArray<int32> VerticesIds;

};
