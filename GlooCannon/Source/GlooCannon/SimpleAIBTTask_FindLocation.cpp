// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAIBTTask_FindLocation.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GlooCannon/SimpleAIController.h"


USimpleAIBTTask_FindLocation::USimpleAIBTTask_FindLocation()
	: SearchRadius(5000.f)
{
	NodeName = TEXT("Find Random Location");

	// accept only vectors in the behavior tree for key names (just nice filterability)
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(USimpleAIBTTask_FindLocation, BlackboardKey));
}

void USimpleAIBTTask_FindLocation::OnGameplayTaskActivated(UGameplayTask& Task)
{
	check(false);
}

EBTNodeResult::Type USimpleAIBTTask_FindLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get ai character
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController is invalid"));
		return EBTNodeResult::Aborted;
	}

	const APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIPawn is invalid"));
		return EBTNodeResult::Aborted;
	}

	// get pawn origin
	const FVector Origin = AIPawn->GetActorLocation();

	// obtain navigation system and find a random location
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation Location{};
	if (!IsValid(NavSystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Navmesh"));
		return EBTNodeResult::Aborted;
	}

	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
		UE_LOG(LogTemp, Warning, TEXT("Setting %s to V(%.2f, %.2f, %.2f)"), *BlackboardKey.SelectedKeyName.ToString(), Location.Location.X, Location.Location.Y, Location.Location.Z);
	}

	// signal behavior tree component that the task finished with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}

FString USimpleAIBTTask_FindLocation::GetStaticDescription() const
{
	// sets the variable underline on the node within the Behavior tree
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
