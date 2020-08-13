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
	std::map<UInfluenceMapNode*, float> unvisitedNodes = std::map<UInfluenceMapNode*, float>();
	std::map<UInfluenceMapNode*, float> visitedNodes = std::map<UInfluenceMapNode*, float>();
	std::vector<float> influenceMapBuffer = std::vector<float>(influenceMap.size());

	unvisitedNodes[currentNode] = 0.0f;

	while (unvisitedNodes.size() > 0)
	{
		std::map<UInfluenceMapNode*, float>::iterator firstElement = unvisitedNodes.begin();
		UInfluenceMapNode* node = firstElement->first;
		float distance = firstElement->second;
		influenceMapBuffer[node->GetIndex()] = 1.0f - (1.0f * (distance / influenceRange));
		visitedNodes[node] = distance;
		for (TPair<UInfluenceMapNode*, float> neighbour : node->GetNeighbours())
		{
			bool inVisited = visitedNodes.find(neighbour.Key) != visitedNodes.end();
			bool inUnvisited = unvisitedNodes.find(neighbour.Key) != unvisitedNodes.end();
			float neighbourDistance = distance + neighbour.Value;
			if (((inVisited && visitedNodes[neighbour.Key] > neighbourDistance) || (inUnvisited && unvisitedNodes[neighbour.Key] > neighbourDistance) || (!inVisited && !inUnvisited)) && neighbourDistance <= influenceRange)
			{
				unvisitedNodes[neighbour.Key] = neighbourDistance;
			}
		}
		unvisitedNodes.erase(node);
	}
	SetInfluenceMap(influenceMapBuffer);
}

float UInfluenceMapPropagator::GetInfluenceRange()
{
	return influenceRange;
}

Team UInfluenceMapPropagator::GetTeam()
{
	return team;
}

