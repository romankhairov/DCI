// Copyright 2024 3D Source, Inc. All Rights Reserved.


#include "MeshOpsActor.h"
#include "ProceduralMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMeshOpsActor::AMeshOpsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComponent");
	ProceduralMeshComponent->SetCollisionProfileName("NoCollision");
	ProceduralMeshComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetCollisionProfileName("NoCollision");
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMeshOpsActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshOpsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
