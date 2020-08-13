// Fill out your copyright notice in the Description page of Project Settings.

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
private:
	UPROPERTY(EditAnywhere)
	float influenceRange;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Team> team;

	std::vector<float> influenceMap;

public:	
	// Sets default values for this component's properties
	UInfluenceMapPropagator();
	UInfluenceMapController* influenceMapController;
	UInfluenceMapNode* currentNode;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	std::vector<float> GetInfluenceMap();
	void SetInfluenceMap(std::vector<float> influenceMapToSet);
	float GetInfluenceRange();
	Team GetTeam();

	void UpdatePropagator();
	void PropagateInfluenceMap();
};
