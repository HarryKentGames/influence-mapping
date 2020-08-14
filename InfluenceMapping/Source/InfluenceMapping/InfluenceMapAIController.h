#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "InfluenceMapPropagator.h"
#include "InfluenceMapAISettings.h"
#include "InfluenceMapAIController.generated.h"


UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	UInfluenceMapPropagator* propagator;
	UInfluenceMapAISettings* aiSettings;
	bool atLocation;
};
