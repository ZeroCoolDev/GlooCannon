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
	if (GlooBlobClass)
	{
		const ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter)
			AddWeightToTarget(OtherCharacter);
		else
			CreateGlooBlob(OtherActor, OtherComp, Hit);
	}

	Destroy();
}

void AGlooCannonProjectile::CreateGlooBlob(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (UWorld* myWorld = GetWorld())
	{
		const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
		AActor* gloo = myWorld->SpawnActor<AActor>(GlooBlobClass, GetActorLocation(), SpawnRotation, SpawnParameters);
		if (gloo)
		{
			if (AGlooBlob* glooBlob = static_cast<AGlooBlob*>(gloo))
				glooBlob->Init(Hit, !OtherActor->IsA(GlooBlobClass));

			if (OtherComp->IsSimulatingPhysics())
			{
				// Apply Gloo Weight
				constexpr bool bWeild = true;
				const FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, bWeild);
				gloo->AttachToActor(OtherActor, attachmentTransformRules);

				OtherComp->AddImpulseAtLocation(-GetVelocity(), Hit.Location);
			}
		}
	}
}

void AGlooCannonProjectile::AddWeightToTarget(const ACharacter* OtherCharacter) const
{
	UE_LOG(LogTemp, Warning, TEXT("spamspam, AddWeightToTarget"));
	if (!OtherCharacter)
		return;

	UWeightComponent* WeightComponent = Cast<UWeightComponent>(OtherCharacter->GetComponentByClass(UWeightComponent::StaticClass()));
	if (WeightComponent)
		WeightComponent->AddWeight();
}
