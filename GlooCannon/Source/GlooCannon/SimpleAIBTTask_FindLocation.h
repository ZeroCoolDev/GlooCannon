// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "SimpleAIBTTask_FindLocation.generated.h"

class UBehaviorTreeComponent;

/**
 * BTTask for finding a random location on a navmesh
 */
UCLASS()
class GLOOCANNON_API USimpleAIBTTask_FindLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	USimpleAIBTTask_FindLocation();


	void OnGameplayTaskActivated(UGameplayTask& Task) override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Search, meta=(AllowPrivateAccess=true))
	float SearchRadius;
};
