// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "WeightComponent.generated.h"

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLOOCANNON_API UWeightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeightComponent();

	void AddWeight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int WeightThreshold;

	int WeightContributorCount;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
