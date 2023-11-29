// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlooCannonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GlooCannon/WeightComponent.h"
#include "GameFramework/Character.h"
#include "GlooCannon/GlooCannonCharacter.h"

AGlooCannonProjectile::AGlooCannonProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.5f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGlooCannonProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AGlooCannonProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GlooBlobClass && !OtherActor->IsA(GlooBlobClass) && !OtherActor->IsA(AGlooCannonCharacter::StaticClass()))
		CreateGlooBlob(OtherActor, OtherComp, Hit);

	Destroy();
}

void AGlooCannonProjectile::CreateGlooBlob(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
	AGlooBlob* GlooBlob = static_cast<AGlooBlob*>(World->SpawnActor<AActor>(GlooBlobClass, GetActorLocation(), SpawnRotation, SpawnParameters));
	if (GlooBlob)
	{
		GlooBlob->AddSplatter(Hit);

		if (OtherComp->IsSimulatingPhysics())
		{
			const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
			GlooBlob->AttachToActor(OtherActor, AttachmentRules);
			OtherComp->AddImpulseAtLocation(-GetVelocity(), Hit.Location);
		}

		if (const ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor))
		{
			GlooBlob->SetActorEnableCollision(false);
			int SocketIndex = FMath::RandRange(0, GlooableSockets.Num() - 1);
			GlooBlob->K2_AttachRootComponentTo(OtherCharacter->GetMesh(), GlooableSockets[SocketIndex], EAttachLocation::SnapToTarget, true);

			AddWeightToTarget(OtherCharacter);
		}
	}
}

void AGlooCannonProjectile::AddWeightToTarget(const ACharacter* OtherCharacter) const
{
	if (!OtherCharacter)
		return;

	UWeightComponent* WeightComponent = Cast<UWeightComponent>(OtherCharacter->GetComponentByClass(UWeightComponent::StaticClass()));
	if (WeightComponent)
		WeightComponent->AddWeight();
}
