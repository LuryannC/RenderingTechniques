// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeVertexPaintingComponent.h"

URuntimeVertexPaintingComponent::URuntimeVertexPaintingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URuntimeVertexPaintingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URuntimeVertexPaintingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
