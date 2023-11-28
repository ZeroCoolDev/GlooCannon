// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GlooBlob.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GLOOCANNON_API AGlooBlob : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlooBlob();

	void AddSplatter(const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
	void ApplySplatterColorEffect(const UDecalComponent* SceneComponent);

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	UMaterial* SplatterDecal;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	float SplatterDepth;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	float SplatterSizeMin;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	float SplatterSizeMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visuals")
	float GlooSizeMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visuals")
	float GlooSizeMax;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GlooSize;
};
