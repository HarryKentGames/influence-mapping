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

UInfluenceMapController* UInfluenceMapPropagator::GetInfluenceMapController()
{
	return influenceMapController;
}

UInfluenceMapNode* UInfluenceMapPropagator::GetCurrentNode()
{
	return currentNode;
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
	//Get the location at the feet of the propagator:
	FVector boundsOrigin = FVector();
	FVector boundsExtents = FVector();
	this->GetOwner()->GetActorBounds(true, boundsOrigin, boundsExtents);
	FVector actorLocation = boundsOrigin;
	actorLocation.Z = actorLocation.Z - boundsExtents.Z;
	//Get the closest node to this location, and if it is different to the current node, update the propagators position:
	UInfluenceMapNode* newNode = influenceMapController->GetClosestNode(actorLocation);
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
			//Check that the neighbour is in influence range:
			float neighbourDistance = distance + neighbour.Value;
			if (neighbourDistance > influenceRange)
			{
				continue;
			}
			//Check that the neighbour has not already been visited, with a shorter distance:
			bool inVisited = visitedNodes.find(neighbour.Key) != visitedNodes.end();
			if (inVisited && visitedNodes[neighbour.Key] < neighbourDistance)
			{
				continue;
			}
			//Check that the neighbour is not currently known about with a shorter distance:
			bool inUnvisited = unvisitedNodes.find(neighbour.Key) != unvisitedNodes.end();
			if (inUnvisited && unvisitedNodes[neighbour.Key] < neighbourDistance)
			{
				continue;
			}
			//If all checks pass, add the node to the list of unvisited nodes:
			unvisitedNodes[neighbour.Key] = neighbourDistance;
		}
		unvisitedNodes.erase(node);
	}
	//Update the influence map:
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

