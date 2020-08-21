#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "InfluenceMapNodeNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "TimerManager.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "InfluenceMapController.generated.h"

class UInfluenceMapPropagator;

UENUM()
enum DebugMapType
{
	Propagator     UMETA(DisplayName = "Propagator"),
	PropagatorAlly      UMETA(DisplayName = "Propagator Ally"),
	PropagatorEnemy      UMETA(DisplayName = "Propagator Enemy"),
	CompleteMap      UMETA(DisplayName = "Complete Map"),
	TensionMap      UMETA(DisplayName = "Tension Map"),
	PropagatorVulnerabilityMap      UMETA(DisplayName = "Propagator Vulnerability Map"),
	PropagatorDirectedVulnerabilityMap      UMETA(DisplayName = "Propagator Directed Vulnerability Map"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapController : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<DebugMapType> debugMapType;

	UInfluenceMapController();

	static UInfluenceMapController* FindInstanceInWorld(UWorld* world);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Getters and Setters:
	UInfluenceMapNodeNetwork* GetNodeNetwork() const;
	TArray<UInfluenceMapNode*> GetNodes() const;
	void AddPropagator(UInfluenceMapPropagator* propagatorToAdd);
	void RemovePropagator(UInfluenceMapPropagator* propagatorToRemove);
	void TargetNewPropagator(int indexOffset);
	UInfluenceMapNode* GetClosestNode(FVector coordinates) const;

	void NormaliseInfluenceMap(std::vector<float>& influenceMap);
	void GetPropagatorInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetPropagatorAllyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetPropagatorEnemyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetCompleteInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetTensionMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);
	void GetDirectedVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap);

	void DebugDraw();

protected:
	virtual void BeginPlay() override;

private:
	UInfluenceMapNodeNetwork* nodeNetwork;
	UPROPERTY()
	TArray<UInfluenceMapNode*> nodes;
	UPROPERTY()
	TArray<UInfluenceMapPropagator*> propagators;
	int targetPropagatorIndex;

	void InitialiseNodeNetwork();
	void UpdatePropagators();
};
