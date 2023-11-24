// Fill out your copyright notice in the Description page of Project Settings.

#include "WeightComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UWeightComponent::UWeightComponent()
	: WeightThreshold(1)
	, WeightContributorCount(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWeightComponent::AddWeight()
{
	if (WeightContributorCount >= WeightThreshold)
		return;

	UE_LOG(LogTemp, Warning, TEXT("spamspam, adding weight"));
	++WeightContributorCount;

	if (MovementComponent)
	{
		const float CurrentMaxWalkSpeed = MovementComponent->MaxWalkSpeed;
		MovementComponent->MaxWalkSpeed = CurrentMaxWalkSpeed * 0.9f;

		UE_LOG(LogTemp, Warning, TEXT("spamspam, reducing movement speed from %.2f to %.2f"), CurrentMaxWalkSpeed, MovementComponent->MaxWalkSpeed);

		if (WeightContributorCount >= WeightThreshold)
			MovementComponent->DisableMovement();
	}
}

// Called when the game starts
void UWeightComponent::BeginPlay()
{
	Super::BeginPlay();

	const ACharacter* Owner = Cast<ACharacter>(GetOwner());
	if (Owner)
		MovementComponent = Owner->GetCharacterMovement();	
}


// Called every frame
void UWeightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

