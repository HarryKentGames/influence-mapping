#pragma once

#include "CoreMinimal.h"
#include <map>
#include "InfluenceMapNode.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INFLUENCEMAPPING_API UInfluenceMapNode : public UObject
{
	GENERATED_BODY()

public:
	UInfluenceMapNode();
	~UInfluenceMapNode();
	static UInfluenceMapNode* MAKE(FVector coordinates, int index);
	void Init(FVector coordinateValue, int indexValue);

	//Getters and Setters:
	void SetCoordinates(FVector coordinates);
	FVector GetCoordinates() const;
	int GetIndex();
	void AddNeighbour(UInfluenceMapNode* neighbour, float distance);
	TMap<UInfluenceMapNode*, float> GetNeighbours();

private:
	UPROPERTY()
	FVector coordinates;
	int index;
	UPROPERTY()
	TMap<UInfluenceMapNode*, float> neighbours;
};
