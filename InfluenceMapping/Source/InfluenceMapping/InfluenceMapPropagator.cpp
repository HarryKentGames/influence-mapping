#include "InfluenceMapPropagator.h"

UInfluenceMapPropagator::UInfluenceMapPropagator()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInfluenceMapPropagator::BeginPlay()
{
	influenceMapController = UInfluenceMapController::FindInstanceInWorld(GetWorld());
	influenceMapController->AddPropagator(this);
}

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
			float neighbourDistance = distance + neighbour.Value;
			if (neighbourDistance > influenceRange)
			{
				continue;
			}
			bool inVisited = visitedNodes.find(neighbour.Key) != visitedNodes.end();
			if (inVisited && visitedNodes[neighbour.Key] < neighbourDistance)
			{
				continue;
			}
			bool inUnvisited = unvisitedNodes.find(neighbour.Key) != unvisitedNodes.end();
			if (inUnvisited && unvisitedNodes[neighbour.Key] < neighbourDistance)
			{
				continue;
			}
			unvisitedNodes[neighbour.Key] = neighbourDistance;
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

