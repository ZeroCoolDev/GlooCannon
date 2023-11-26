// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "GlooBlob.generated.h"

UCLASS(Blueprintable, BlueprintType)
class GLOOCANNON_API AGlooBlob : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlooBlob();

	void Init(const FHitResult& Hit, bool bAddSplatter);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateSizeEffect(const USceneComponent* SceneComponent, float MaxSize);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateGlooColorEffect(const UPrimitiveComponent* SceneComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateSplatterColorEffect(const UDecalComponent* SceneComponent);

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	UMaterial* Decal;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category="Decal")
	float DecalDepth;

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

};
