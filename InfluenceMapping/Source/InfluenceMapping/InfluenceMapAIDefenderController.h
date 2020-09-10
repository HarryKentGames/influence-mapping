#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "InfluenceMapPropagator.h"
#include "InfluenceMapAIController.h"
#include "InfluenceMapAISettings.h"
#include "InfluenceMapAIDefenderController.generated.h"


UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapAIDefenderController : public AInfluenceMapAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	bool movingToInterceptAttacker;
	bool movingToPatrolLocation;

	void DoDefenderLogic(UInfluenceMapNode* currentNode);
};
