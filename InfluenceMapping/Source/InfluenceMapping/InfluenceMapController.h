// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "InfluenceMapNodeNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "TimerManager.h"
#include "InfluenceMapController.generated.h"

class UInfluenceMapPropagator;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapController : public USceneComponent
{
	GENERATED_BODY()
private:
	UInfluenceMapNodeNetwork* nodeNetwork;
	TArray<UInfluenceMapNode*> nodes;
	TArray<UInfluenceMapPropagator*> propagators;

	void InitialiseNodeNetwork();
	void UpdatePropagators();
	void PropagateInfluences();

public:
	UPROPERTY(EditAnywhere)
	bool debugDraw;

	// Sets default values for this component's properties
	UInfluenceMapController();
	static UInfluenceMapController* FindInstanceInWorld(UWorld* world);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UInfluenceMapNode*> GetNodes();
	UInfluenceMapNode* GetClosestNode(FVector coordinates);
	void AddPropagator(UInfluenceMapPropagator* propagatorToAdd);
	void RemovePropagator(UInfluenceMapPropagator* propagatorToRemove);
	
	void DebugDraw();
};
