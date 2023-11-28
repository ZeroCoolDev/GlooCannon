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
	const FVector SplatterLocation= Hit.ImpactPoint;

	FRotator SplatterRotation = Hit.ImpactNormal.Rotation();
	SplatterRotation.Roll = FMath::FRandRange(-180.f, 180.f);

	const float SplatterSize = ((GlooSize - GlooSizeMin) / (GlooSizeMax - GlooSizeMin)) * (SplatterSizeMax - SplatterSizeMin) + SplatterSizeMin;
	const FVector SplatterBounds = FVector(SplatterDepth, SplatterSize, SplatterSize);

	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached(SplatterDecal, SplatterBounds, Hit.GetComponent(), NAME_None, SplatterLocation, SplatterRotation, EAttachLocation::KeepWorldPosition);
	if (DecalComponent)
	{
		DecalComponent->SetFadeIn(0.25f, 0.4f);
		ApplySplatterColorEffect(DecalComponent);
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

