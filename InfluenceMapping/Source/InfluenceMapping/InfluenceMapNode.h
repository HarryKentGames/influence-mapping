// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "InfluenceMapNode.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INFLUENCEMAPPING_API UInfluenceMapNode : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	FVector coordinates;
	int index;
	UPROPERTY(VisibleAnywhere)
	TMap<UInfluenceMapNode*, float> neighbours;

public:
	UInfluenceMapNode();
	~UInfluenceMapNode();
	
	static UInfluenceMapNode* MAKE(FVector coordinates, int index);

	void Init(FVector coordinateValue, int indexValue);
	void SetCoordinates(FVector coordinates);
	FVector GetCoordinates() const;
	int GetIndex();
	void AddNeighbour(UInfluenceMapNode* neighbour, float distance);
	TMap<UInfluenceMapNode*, float> GetNeighbours();
};
