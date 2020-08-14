#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceMapController.h"
#include "InfluenceMapPropagator.generated.h"

UENUM()
enum Team
{
	Team1     UMETA(DisplayName = "Team1"),
	Team2      UMETA(DisplayName = "Team2"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapPropagator : public UActorComponent
{
	GENERATED_BODY()

public:
	UInfluenceMapController* influenceMapController;
	UInfluenceMapNode* currentNode;

	UInfluenceMapPropagator();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	std::vector<float> GetInfluenceMap();
	void SetInfluenceMap(std::vector<float> influenceMapToSet);
	float GetInfluenceRange();
	Team GetTeam();

	void UpdatePropagator();
	void PropagateInfluenceMap();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float influenceRange;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<Team> team;
	std::vector<float> influenceMap;
};
