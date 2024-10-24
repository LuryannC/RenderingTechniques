// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "RenderResource.h"
#include "StaticMeshComponentLODInfo.h"

ATestActor::ATestActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TestMesh"));
}

void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	OnAsyncTaskFinished.AddUFunction(this, FName("OnAsyncTaskFinishedFunc"));
}

void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::PaintSMVertices()
{
	if (!StaticMeshComponent) return;

	
	if (UStaticMesh* Mesh = StaticMeshComponent->GetStaticMesh())
	{
		//Make sure that we have at least 1 LOD
		StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
		FStaticMeshComponentLODInfo* LODInfo = &StaticMeshComponent->LODData[0];

		//Empty the painted vertices and assign a new color vertex buffer which will contain the new colors for each vertex
		LODInfo->PaintedVertices.Empty();
		LODInfo->OverrideVertexColors = new FColorVertexBuffer();

		//We're going to use the LODResources to get the total number of vertices that the provided mesh has
		FStaticMeshLODResources& LODResources = Mesh->GetRenderData()->LODResources[0];

		//Creating a color array
		TArray<FColor> RandomColorArray;

		//Since we know beforehand the number of elements we might as well reserve the memory now
		RandomColorArray.Reserve(LODResources.GetNumVertices() - 1);

		for (int32 i = 0; i < LODResources.GetNumVertices(); i++)
		{
			//Generate a random color for the current vertex
			RandomColorArray.Add(FColor::MakeRandomColor());
		}

		//Initialize the new vertex colros with the array we created above
		LODInfo->OverrideVertexColors->InitFromColorArray(RandomColorArray);

		//Initialize resource and mark render state of object as dirty in order for the engine to re-render it
		BeginInitResource(LODInfo->OverrideVertexColors);
		StaticMeshComponent->MarkRenderStateDirty();
	}	
}

UStaticMesh* ATestActor::DuplicateStaticMesh(UStaticMesh* OriginalMesh)
{
	if (!OriginalMesh) return nullptr;
	
	UStaticMesh* DuplicatedMesh = DuplicateObject<UStaticMesh>(OriginalMesh, nullptr);

	if (DuplicatedMesh)
	{
		// Mark the duplicated mesh as transient (so it doesn't get saved)
		DuplicatedMesh->SetFlags(RF_Transient);
	}

	return DuplicatedMesh;
}

void ATestActor::PaintSMVerticesAtLocation(FVector Location, FColor Color, float Radius)
{
	if (!StaticMeshComponent) return;

    UStaticMesh* OriginalMesh = StaticMeshComponent->GetStaticMesh();
    if (!OriginalMesh) return;

    // Duplicate the static mesh so we can modify its vertex colors
    UStaticMesh* DuplicatedMesh = DuplicateStaticMesh(OriginalMesh);
    if (!DuplicatedMesh) return;
	
    // Access LOD 0 (or another LOD if necessary)
    FStaticMeshRenderData* RenderData = DuplicatedMesh->GetRenderData();
    if (!RenderData || RenderData->LODResources.Num() == 0) return;

    FStaticMeshLODResources& LODResource = RenderData->LODResources[0];
	
	StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
	FStaticMeshComponentLODInfo* LODInfo = &StaticMeshComponent->LODData[0];

    // Get the position buffer (vertex positions) and color buffer (vertex colors)
    FPositionVertexBuffer& PositionBuffer = LODResource.VertexBuffers.PositionVertexBuffer;
    FColorVertexBuffer& ColorBuffer = LODResource.VertexBuffers.ColorVertexBuffer;

    // Ensure that the ColorBuffer is initialized
    if (ColorBuffer.GetNumVertices() == 0)
    {
        ColorBuffer.InitFromSingleColor(FColor::White, PositionBuffer.GetNumVertices());
    }

    // Create a writable copy of the color data
    TArray<FColor> VertexColors;

	//VertexColors.SetNum(ColorBuffer.GetNumVertices());

	VertexColors.Reserve(LODResource.GetNumVertices() - 1);

	for (int32 i = 0; i < LODResource.GetNumVertices(); i++)
	{
		FVector3f VertexPosition = PositionBuffer.VertexPosition(i);
    
		float Distance = FVector3f::Dist(VertexPosition, FVector3f(Location));

		if (Distance <= Radius)
		{
			VertexColors.Add(Color);
		}
		else
		{
			// Keep the original color or use a default color or whatever the default is
			VertexColors.Add(FColor::White);
		}
	}

	LODInfo->OverrideVertexColors->InitFromColorArray(VertexColors);

	BeginInitResource(LODInfo->OverrideVertexColors);
	StaticMeshComponent->MarkRenderStateDirty();
	
 //    // Copy existing vertex colors
 //    for (uint32 i = 0; i < ColorBuffer.GetNumVertices(); i++)
 //    {
 //        VertexColors[i] = ColorBuffer.VertexColor(i);
 //    }
 //
	// for (uint32 i = 0; i < PositionBuffer.GetNumVertices(); i++)
	// {
	// 	FVector3f VertexPosition = PositionBuffer.VertexPosition(i);
 //
	// 	// Calculate the distance from the vertex to the hit location
	// 	float Distance = FVector3f::Dist(VertexPosition, FVector3f(Location));
 //
	// 	// Check if the vertex is within the given radius
	// 	if (Distance <= Radius)
	// 	{
	// 		// Option 1: Uniform color change (all vertices within radius get the same color)
	// 		VertexColors[i] = Color;
 //
	// 		// Option 2: Gradual color change based on distance (falloff effect)
	// 		// float FalloffFactor = FMath::Clamp(1.0f - (Distance / Radius), 0.0f, 1.0f);
	// 		// VertexColors[i] = FLinearColor::LerpUsingHSV(FLinearColor(VertexColors[i]), FLinearColor(PaintColor), FalloffFactor).ToFColor(true);
	// 	}
	// }
 //
 //    // Update the color buffer with the new colors
 //    ColorBuffer.InitFromColorArray(VertexColors);
 //
 //    // Assign the modified mesh back to the static mesh component
 //    StaticMeshComponent->SetStaticMesh(DuplicatedMesh);
 //
 //    // Mark the component to indicate that its render state should be updated
 //    StaticMeshComponent->MarkRenderStateDirty();
}

void ATestActor::PaintVerticesAtLocation(FVector HitLocation, FColor TargetBaseColor, float PaintLerpProgress, float BrushSize)
{
	if (!StaticMeshComponent) return;
	
	if (UStaticMesh* Mesh = StaticMeshComponent->GetStaticMesh())
	{
		if (StaticMeshComponent->LODData.IsEmpty())
		{
			StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
		}
		FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
		FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];

		InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
		if (ExistingColors.Num() > 0)
		{
			InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(ExistingColors);
		}
		else
		{
			InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, LODModel.GetNumVertices());
		}
		
		// if (InstanceMeshLODInfo->OverrideVertexColors == nullptr)
		// {
		// 	InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;
		// 	InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(FColor::White, LODModel.GetNumVertices());
		// 	//BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
		// }
		//
		BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
		
		FPositionVertexBuffer& PositionBuffer = LODModel.VertexBuffers.PositionVertexBuffer;
		FColorVertexBuffer& ColorBuffer = *InstanceMeshLODInfo->OverrideVertexColors;
		
		
		ExistingColors.SetNumUninitialized(LODModel.GetNumVertices());

		for (int32 VertexIndex = 0; VertexIndex < LODModel.GetNumVertices(); VertexIndex++)
		{
			ExistingColors[VertexIndex] = ColorBuffer.VertexColor(VertexIndex); // Retain the current color of each vertex
		}
				
		DrawDebugSphere(GetWorld(), HitLocation, BrushSize, 32, FColor::Green, false, 5.0f);
		
		const int32 NumVertices = LODModel.GetNumVertices();
		FTransform MeshTransform = StaticMeshComponent->GetComponentTransform();

		Async(EAsyncExecution::TaskGraph, [this, &LODModel, &PositionBuffer, &ColorBuffer, NumVertices, HitLocation, TargetBaseColor, PaintLerpProgress, BrushSize, MeshTransform]()
		{
			UE_LOG(LogTemp, Display, TEXT("ATestActor::PaintVerticesAtLocation - Async work Started"));
			
			// Heavy computation: paint vertices in the background thread
			VerticesIds.Empty();
			for (int32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
			{
				FVector3f VertexPositionLocal = PositionBuffer.VertexPosition(VertexIndex);
				FVector VertexPositionWorld = MeshTransform.TransformPosition(FVector(VertexPositionLocal));
				
				float Distance = FVector::Dist(HitLocation, VertexPositionWorld);

				if (Distance <= BrushSize)
				{
					VerticesIds.Add(VertexIndex);

					// Calculate the influence based on distance (falloff: linear)
					float Influence = 1.0f - (Distance / BrushSize);

					// Lerp from the previous color to the target base color based on the influence
					FColor NewColor;
					NewColor.R = 255;
					NewColor.G = 0;
					NewColor.B = 0;
					
					UE_LOG(LogTemp, Display, TEXT("ATestActor::PaintVerticesAtLocation - Painting Vertex %d with Color: R: %d G: %d B: %d"), VertexIndex, NewColor.R, NewColor.G, NewColor.B);

					ExistingColors[VertexIndex] = NewColor;
					ColorBuffer.VertexColor(VertexIndex) = NewColor;
					UE_LOG(LogTemp, Display, TEXT("ATestActor::PaintVerticesAtLocation - Vertex %d new color after applied: R=%i G=%i B=%i"), VertexIndex, ColorBuffer.VertexColor(VertexIndex).R, ColorBuffer.VertexColor(VertexIndex).G, ColorBuffer.VertexColor(VertexIndex).B);
				}
			}
			
			// // Once painting is done, make sure the render state is updated on the game thread
			AsyncTask(ENamedThreads::GameThread, [this]()
			{
				
				OnAsyncTaskFinished.Broadcast();								
				UE_LOG(LogTemp, Display, TEXT("ATestActor::PaintVerticesAtLocation - Async work finished"));
			});
		});
	}
}

void ATestActor::GetVertexColorsinArea(FVector HitLocation, float Radius)
{
	if (!StaticMeshComponent) return;

	if (UStaticMesh* Mesh = StaticMeshComponent->GetStaticMesh())
	{
		if (StaticMeshComponent->LODData.IsEmpty())
		{
			StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
		}
		FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
		FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];

		DrawDebugSphere(GetWorld(), HitLocation, Radius, 32, FColor::Green, false, 5.0f);

		FPositionVertexBuffer& PositionBuffer = LODModel.VertexBuffers.PositionVertexBuffer;

		const int32 NumVertices = LODModel.GetNumVertices();
		FTransform MeshTransform = StaticMeshComponent->GetComponentTransform();

		Async(EAsyncExecution::TaskGraph, [this, &LODModel, &PositionBuffer, NumVertices, HitLocation, MeshTransform]()
		{
			for (int32 VertexIndex = 0; VertexIndex < NumVertices; VertexIndex++)
			{
				FVector3f VertexPositionLocal = PositionBuffer.VertexPosition(VertexIndex);
				FVector VertexPositionWorld = MeshTransform.TransformPosition(FVector(VertexPositionLocal));
				
				float Distance = FVector::Dist(HitLocation, VertexPositionWorld);
			}
		});
	}
}

void ATestActor::OnAsyncTaskFinishedFunc()
{
	StaticMeshComponent->LODData[0].OverrideVertexColors->InitFromColorArray(ExistingColors);
	StaticMeshComponent->MarkRenderStateDirty();
}
