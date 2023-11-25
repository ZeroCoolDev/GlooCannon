// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SimpleAIController.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class GLOOCANNON_API ASimpleAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ASimpleAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=AI, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
