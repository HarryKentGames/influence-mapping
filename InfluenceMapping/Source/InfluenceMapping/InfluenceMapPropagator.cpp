// Fill out your copyright notice in the Description page of Project Settings.


#include "InfluenceMapPropagator.h"

// Sets default values for this component's properties
UInfluenceMapPropagator::UInfluenceMapPropagator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInfluenceMapPropagator::BeginPlay()
{
	influenceMapController = UInfluenceMapController::FindInstanceInWorld(GetWorld());
	influenceMapController->AddPropagator(this);
}


// Called every frame
void UInfluenceMapPropagator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

std::vector<float> UInfluenceMapPropagator::GetInfluenceMap()
{
	return influenceMap;
}

void UInfluenceMapPropagator::SetInfluenceMap(std::vector<float> influenceMapToSet)
{
	influenceMap = influenceMapToSet;
}

void UInfluenceMapPropagator::UpdatePropagator()
{
	UInfluenceMapNode* newNode = influenceMapController->GetClosestNode(this->GetOwner()->GetActorLocation());
	if (currentNode != nullptr && newNode != currentNode)
	{
		influenceMap[currentNode->GetIndex()] = 0.0f;
	}
	currentNode = newNode;
	influenceMap[currentNode->GetIndex()] = 1.0f;
}

void UInfluenceMapPropagator::PropagateInfluenceMap()
{
	std::map<UInfluenceMapNode*, float> visitedNodes = std::map<UInfluenceMapNode*, float>();
	std::vector<float> influenceMapBuffer = std::vector<float>(influenceMap.size());
	PropagateInfluence(currentNode, 0.0f, influenceMapBuffer, visitedNodes);
	SetInfluenceMap(influenceMapBuffer);
}

void UInfluenceMapPropagator::PropagateInfluence(UInfluenceMapNode* node, float totalDistance, std::vector<float> &influenceMapBuffer, std::map<UInfluenceMapNode*, float> &visitedNodes)
{
	//If the node is out of range, stop:
	if (totalDistance > influenceRange || node == nullptr)
	{
		return;
	}
	//Calculate the influence for the node, and update the map:
	float influence = 1 - (1 * (totalDistance / influenceRange));
	visitedNodes[node] = influence;
	influenceMapBuffer[node->GetIndex()] = influence;
	//Loop over all the neighbours:
	for (TPair<UInfluenceMapNode*, float> neighbourPair : node->GetNeighbours())
	{
		UInfluenceMapNode* neighbourNode = neighbourPair.Key;
		float distance = neighbourPair.Value;
		float neighbourInfluence = 1 - (1 * (totalDistance / influenceRange));
		bool neighbourVisited = visitedNodes.find(neighbourNode) != visitedNodes.end();
		//If the neighbour hasnt been visited, or its new influence would be lower, propogate influence for the neighbour:
		if (!neighbourVisited || (neighbourVisited && visitedNodes[neighbourNode] < neighbourInfluence))
		{
			PropagateInfluence(neighbourNode, totalDistance + distance, influenceMapBuffer, visitedNodes);
		}
	}
}

float UInfluenceMapPropagator::GetInfluenceRange()
{
	return influenceRange;
}

