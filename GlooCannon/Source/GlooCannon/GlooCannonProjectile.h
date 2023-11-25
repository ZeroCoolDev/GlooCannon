// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlooCannon/GlooBlob.h"

#include "GlooCannonProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AGlooCannonProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AGlooBlob> GlooBlobClass;

public:
	AGlooCannonProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void CreateGlooBlob(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	void AddWeightToTarget(const ACharacter* OtherCharacter) const;

private:
	const TArray<FName> GlooableSockets = TArray<FName> { FName("ghostBeast_clavicle_r"), FName("ghostBeast_clavicle_l"), FName("ghostBeast_pelvis"), FName("ghostBeast_upperarm_twist_01_l"), FName("ghostBeast_upperarm_twist_01_r"), FName("ghostBeast_head"), FName("ghostBeast_thigh_l"), FName("ghostBeast_thigh_r") };
};

