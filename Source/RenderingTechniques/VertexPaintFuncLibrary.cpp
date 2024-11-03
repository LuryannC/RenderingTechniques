// Fill out your copyright notice in the Description page of Project Settings.


#include "VertexPaintFuncLibrary.h"

#include "RuntimeVertexPaintingComponent.h"
#include "StaticMeshComponentLODInfo.h"



// void UVertexPaintFuncLibrary::PaintVerticesAtLocation(URuntimeVertexPaintingComponent* VertexPaintingComponent, FVector Location,
//                                                       float Radius, FColor ColorToPaint, bool bIgnoreVertexNormal)
// {
// 	UStaticMeshComponent* StaticMeshComponent = VertexPaintingComponent->GetStaticMeshComponent(); 
//
// 	if (!StaticMeshComponent) return;
//
// 	if (UStaticMesh* Mesh = StaticMeshComponent->GetStaticMesh())
// 	{
// 		if (StaticMeshComponent->LODData.IsEmpty())
// 		{
// 			StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
// 		}
// 		
// 		FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
// 		FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];
//
// 		InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
//
// 		if (InstanceMeshLODInfo->PaintedVertices.Num() > 0)
// 		{
// 			for (auto Vertex : InstanceMeshLODInfo->PaintedVertices)
// 			{
// 				VertexPaintingComponent->ExistingColors.Add(Vertex.Color);
// 			}
// 		}
// 		
// 		if (VertexPaintingComponent->ExistingColors.Num() > 0)
// 		{
// 			InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(VertexPaintingComponent->ExistingColors);
// 		}
// 		else
// 		{
// 			InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, LODModel.GetNumVertices());
// 		}
//
// 		BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
// 		
// 		FPositionVertexBuffer& PositionBuffer = LODModel.VertexBuffers.PositionVertexBuffer;
// 		FColorVertexBuffer& ColorBuffer = *InstanceMeshLODInfo->OverrideVertexColors;
//
// 		VertexPaintingComponent->ExistingColors.SetNumUninitialized(LODModel.GetNumVertices());
//
// 		for (int32 VertexIndex = 0; VertexIndex < LODModel.GetNumVertices(); VertexIndex++)
// 		{
// 			VertexPaintingComponent->ExistingColors[VertexIndex] = ColorBuffer.VertexColor(VertexIndex);
// 		}
//
// 		DrawDebugSphere(VertexPaintingComponent->GetWorld(), Location, Radius, 32, FColor::Green, false, 1.0f);
// 		
// 		const int32 NumVertices = LODModel.GetNumVertices();
// 		FTransform MeshTransform = StaticMeshComponent->GetComponentTransform();		
//
// 		Async(EAsyncExecution::TaskGraph, [&PositionBuffer, &ColorBuffer, NumVertices, Location, ColorToPaint, Radius, MeshTransform, VertexPaintingComponent, StaticMeshComponent, bIgnoreVertexNormal]()
// 		{
// 			for (int32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
// 			{
// 				FVector3f VertexPositionLocal = PositionBuffer.VertexPosition(VertexIndex);
// 				FVector VertexPositionWorld = MeshTransform.TransformPosition(FVector(VertexPositionLocal));
//
// 				float Distance = FVector::Dist(Location, VertexPositionWorld);
// 				
// 				if (Distance <= Radius)
// 				{
// 					FColor NewColor = ColorToPaint;
// 					VertexPaintingComponent->ExistingColors[VertexIndex] = NewColor;
// 					ColorBuffer.VertexColor(VertexIndex) = NewColor;					
// 				}
// 			}
//
// 			AsyncTask(ENamedThreads::GameThread, [StaticMeshComponent, VertexPaintingComponent]()
// 			{				
// 				StaticMeshComponent->LODData[0].OverrideVertexColors->InitFromColorArray(VertexPaintingComponent->ExistingColors);
// 				StaticMeshComponent->MarkRenderStateDirty();							
// 			});
// 		});
// 	}
// }

void UVertexPaintFuncLibrary::PaintVerticesAtLocationV2(URuntimeVertexPaintingComponent* VertexPaintingComponent,
	FVector Location, float Radius, FColor ColorToPaint)
{
	if (!VertexPaintingComponent) return;

	VertexPaintingComponent->InitializeVertexStructure();
	
	VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors = new FColorVertexBuffer;
	
	if (VertexPaintingComponent->VerticesStructure.LODInfo->PaintedVertices.Num() > 0)
	{
		// load the already painted so we don't override it
		for (auto Vertex : VertexPaintingComponent->VerticesStructure.LODInfo->PaintedVertices)
		{
			VertexPaintingComponent->Colors.Add(Vertex.Color);
		}
	}

	// If this mesh was already painted we just add the colors to the vertices, if not then just create a new one.
	if (VertexPaintingComponent->Colors.Num() > 0)
	{
		VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors->InitFromColorArray(VertexPaintingComponent->Colors);
	}
	else
	{
		VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, VertexPaintingComponent->VerticesStructure.LODModel->GetNumVertices());
	}
		
	BeginInitResource(VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors);
	
	VertexPaintingComponent->Colors.SetNumUninitialized(VertexPaintingComponent->VerticesStructure.LODModel->GetNumVertices());
	for (int32 VertexIndex = 0; VertexIndex < VertexPaintingComponent->VerticesStructure.LODModel->GetNumVertices(); VertexIndex++)
	{
		VertexPaintingComponent->Colors[VertexIndex] = VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors->VertexColor(VertexIndex);
	}
	
	// Execute Task
	Async(EAsyncExecution::TaskGraph, [VertexPaintingComponent, Location, Radius, ColorToPaint]()
	{
		const int32 NumVertices = VertexPaintingComponent->VerticesStructure.LODModel->GetNumVertices();
		FTransform MeshTransform = VertexPaintingComponent->GetStaticMeshComponent()->GetComponentTransform();

		for (int32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
		{
			FVector3f VertexPositionLocal = VertexPaintingComponent->VerticesStructure.LODModel->VertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex);
			FVector VertexPositionWorld = MeshTransform.TransformPosition(FVector(VertexPositionLocal));

			float Distance = FVector::Dist(Location, VertexPositionWorld);
			
			if (Distance <= Radius)
			{
				FColor NewColor = ColorToPaint;
				VertexPaintingComponent->Colors[VertexIndex] = NewColor;
				VertexPaintingComponent->VerticesStructure.LODInfo->OverrideVertexColors->VertexColor(VertexIndex) = NewColor;					
			}
		}
		AsyncTask(ENamedThreads::GameThread, [VertexPaintingComponent]()
		{				
			VertexPaintingComponent->GetStaticMeshComponent()->LODData[0].OverrideVertexColors->InitFromColorArray(VertexPaintingComponent->Colors);
			VertexPaintingComponent->GetStaticMeshComponent()->MarkRenderStateDirty();							
		});
	});
}
