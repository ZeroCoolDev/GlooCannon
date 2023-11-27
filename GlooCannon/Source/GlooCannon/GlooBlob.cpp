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

void AGlooBlob::AddSplatter(const FHitResult& Hit)
{
	const FVector Location = Hit.ImpactPoint;
	FRotator RandomRollRotation = Hit.ImpactNormal.Rotation();
	RandomRollRotation.Roll = FMath::FRandRange(-180.f, 180.f);

	const float RandomDecalSize = SplatterSize;//DecalSizeMod;
	const FVector DecalBounds = FVector(SplatterDepth, RandomDecalSize, RandomDecalSize);

	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached(SplatterDecal, DecalBounds, Hit.GetComponent(), NAME_None, Location, RandomRollRotation, EAttachLocation::KeepWorldPosition);
	if (DecalComponent)
	{
		DecalComponent->SetFadeIn(0.25f, 0.4f);
		CreateSplatterColorEffect(DecalComponent);
	}
}

// Called when the game starts or when spawned
void AGlooBlob::BeginPlay()
{
	Super::BeginPlay();

	GlooSize = FMath::FRandRange(GlooSizeMin, GlooSizeMax);
	CreateSizeEffect(StaticMesh, GlooSize);
	CreateGlooColorEffect(StaticMesh);

	SplatterSize = ((GlooSize - GlooSizeMin) / (GlooSizeMax - GlooSizeMin)) * (SplatterSizeMax - SplatterSizeMin) + SplatterSizeMin;
}

// Called every frame
void AGlooBlob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

