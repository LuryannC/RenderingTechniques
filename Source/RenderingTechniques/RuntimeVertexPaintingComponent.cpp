// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeVertexPaintingComponent.h"

#include "StaticMeshComponentLODInfo.h"

void URuntimeVertexPaintingComponent::BeginPlay()
{
	Super::BeginPlay();

	//InitializeVertexStructure();
	//InitializeColourData();
}

UStaticMeshComponent* URuntimeVertexPaintingComponent::GetStaticMeshComponent()
{
	TArray<UActorComponent*> OutStaticMeshes;
	GetOwner()->GetComponents(UStaticMeshComponent::StaticClass(), OutStaticMeshes);

	if (!OutStaticMeshes.IsEmpty())
	{
		return Cast<UStaticMeshComponent>(OutStaticMeshes[0]);
	}
	return nullptr;
}

USkeletalMeshComponent* URuntimeVertexPaintingComponent::GetSkeletalMeshComponent()
{
	TArray<USkeletalMeshComponent*> OutSkeletalMeshes;
	GetOwner()->GetComponents(USkeletalMeshComponent::StaticClass(), OutSkeletalMeshes);

	if (!OutSkeletalMeshes.IsEmpty())
	{
		return Cast<USkeletalMeshComponent>(OutSkeletalMeshes[0]);
	}
	return nullptr;
}

void URuntimeVertexPaintingComponent::InitializeVertexStructure()
{	
	VerticesStructure.VertexPaintingComponent = this;
	VerticesStructure.StaticMeshComponent = GetStaticMeshComponent();

	if (VerticesStructure.StaticMeshComponent && VerticesStructure.StaticMeshComponent->GetStaticMesh())
	{
		if (GetStaticMeshComponent()->LODData.IsEmpty())
		{
			GetStaticMeshComponent()->SetLODDataCount(1, GetStaticMeshComponent()->LODData.Num());
		}
		VerticesStructure.LODModel = &GetStaticMeshComponent()->GetStaticMesh()->GetRenderData()->LODResources[0];
		VerticesStructure.LODInfo = &GetStaticMeshComponent()->LODData[0];
	}
}

void URuntimeVertexPaintingComponent::InitializeColourData()
{
}
