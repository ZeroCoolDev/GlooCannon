// Fill out your copyright notice in the Description page of Project Settings.


#include "GlooBlob.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGlooBlob::AGlooBlob()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollision->SetGenerateOverlapEvents(true);
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(BoxCollision);
}

void AGlooBlob::Init(const FHitResult& Hit, bool bAddSplatter)
{
	const float GlooMin = 0.2f, GlooMax = 0.1f;
	const float GlooSizeMod = FMath::FRandRange(GlooMin, GlooMax);
	CreateSizeEffect(StaticMesh, GlooSizeMod);
	CreateGlooColorEffect(StaticMesh);

	if (bAddSplatter)
	{
		// Keeps the gloo size and the decal size consistent (evne though they have their own ranges)
		const float DecalMin = 0.75f, DecalMax = 1.15f;
		const float DecalSizeMod = ((GlooSizeMod - GlooMin) / (GlooMax - GlooMin)) * (DecalMax - DecalMin) + DecalMin;

		const FVector Location = Hit.ImpactPoint;
		FRotator RandomRollRotation = Hit.ImpactNormal.Rotation();
		RandomRollRotation.Roll = FMath::FRandRange(-180.f, 180.f);

		const float RandomDecalSize = DecalSize * DecalSizeMod;
		const FVector DecalBounds = FVector(DecalDepth, RandomDecalSize, RandomDecalSize);

		UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached(Decal, DecalBounds, Hit.GetComponent(), NAME_None, Location, RandomRollRotation, EAttachLocation::KeepWorldPosition);
		if (DecalComponent)
		{
			DecalComponent->SetFadeIn(0.25f, 0.4f);
			// In testing I can't decide if it looks better with the splatter growing in size of not but for now leave it out
			// CreateSizeEffect(DecalComponent, DecalSizeMod);
			CreateSplatterColorEffect(DecalComponent);
		}
	}
}

// Called when the game starts or when spawned
void AGlooBlob::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlooBlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

