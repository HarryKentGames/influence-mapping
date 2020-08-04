#include "InfluenceMapNode.h"

UInfluenceMapNode::UInfluenceMapNode()
	: coordinates(FVector()), index(0)
{
};

UInfluenceMapNode::~UInfluenceMapNode()
{
}

UInfluenceMapNode* UInfluenceMapNode::MAKE(FVector coordinates, int index)
{
	UInfluenceMapNode* node = NewObject<UInfluenceMapNode>();
	node->Init(coordinates, index);
	return node;
}


void UInfluenceMapNode::Init(FVector coordinateValue, int indexValue)
{
	coordinates = coordinateValue;
	index = indexValue;
};

void UInfluenceMapNode::SetCoordinates(FVector coordinateValue)
{
	coordinates = coordinateValue;
}

FVector UInfluenceMapNode::GetCoordinates() const
{
	return coordinates;
}

int UInfluenceMapNode::GetIndex()
{
	return index;
}

void UInfluenceMapNode::AddNeighbour(UInfluenceMapNode* neighbour, float distance)
{
	neighbours.Add(neighbour, distance);
}

TMap<UInfluenceMapNode*, float> UInfluenceMapNode::GetNeighbours()
{
	return neighbours;
}
