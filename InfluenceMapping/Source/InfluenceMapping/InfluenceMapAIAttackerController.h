#pragma once

#include "CoreMinimal.h"
#include "InfluenceMapAIController.h"
#include "InfluenceMapAIAttackerController.generated.h"

UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapAIAttackerController : public AInfluenceMapAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	bool movingToFlag;
	bool movingToSafeLocation;

	void DoAttackerLogic(UInfluenceMapNode* currentNode);
};
