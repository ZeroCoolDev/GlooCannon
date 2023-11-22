// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlooCannonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"

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
	if (GlooBlobClass && !OtherActor->IsA(GlooBlobClass))
	{
		const ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
		if (OtherCharacter)
		{
			// Apply gloo effect to character
		}
		else
			CreateGlooBlob(OtherActor, OtherComp, Hit);
	}

	Destroy();
	// TODO: add back in a concept of weight for the object the gloo hits
}

void AGlooCannonProjectile::CreateGlooBlob(AActor* OtherACtor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (UWorld* myWorld = GetWorld())
	{
		const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
		AActor* gloo = myWorld->SpawnActor<AActor>(GlooBlobClass, GetActorLocation(), SpawnRotation, SpawnParameters);
		if (gloo && OtherComp->IsSimulatingPhysics())
		{
			constexpr bool bWeild = true;
			const FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, bWeild);

			gloo->AttachToActor(OtherACtor, attachmentTransformRules);

			OtherComp->AddImpulseAtLocation(-GetVelocity() * 10.f, Hit.Location);
			
			// the impulse velocity is CONSTANTLY being applied versus an impulse...
			// it's definitely because we're attaching the two objects but I am honeslty not sure why
			UE_LOG(LogTemp, Warning, TEXT("spamspam, %.2f, %.2f, %.2f"), GetVelocity().X, GetVelocity().Y, GetVelocity().Z);
			//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, Hit.Location);
			UE_LOG(LogTemp, Warning, TEXT("spamspam"));
		}
	}
}
