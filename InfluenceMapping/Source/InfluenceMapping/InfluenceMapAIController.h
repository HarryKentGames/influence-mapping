// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "InfluenceMapPropagator.h"
#include "InfluenceMapAIController.generated.h"

/**
 * 
 */
UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapAIController : public AAIController
{
	GENERATED_BODY()
private:
	UInfluenceMapPropagator* propagator;
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

};
