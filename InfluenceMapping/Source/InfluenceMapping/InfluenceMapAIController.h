#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "InfluenceMapPropagator.h"
#include "InfluenceMapAIController.h"
#include "InfluenceMapAISettings.h"
#include "InfluenceMapAIController.generated.h"

UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	AActor* actor;
	TArray<AInfluenceMapAIController*> teamMateControllers;

	UInfluenceMapPropagator* propagator;
	UInfluenceMapAISettings* aiSettings;
	UInfluenceMapNode* locationNode;

	UInfluenceMapNode* FindClosestNode(TArray<UInfluenceMapNode*> nodes, UInfluenceMapNode* currentNode);
	bool IsTeamMateDealingWithNearbyNode(UInfluenceMapNode* node, TMap<UInfluenceMapPropagator*, UInfluenceMapNode*> teamMateDestinations);

};
