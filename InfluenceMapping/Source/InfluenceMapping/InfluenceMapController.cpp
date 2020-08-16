#include "InfluenceMapController.h"
#include "InfluenceMapPropagator.h"

UInfluenceMapController::UInfluenceMapController()
{
	PrimaryComponentTick.bCanEverTick = true;
}

UInfluenceMapController* UInfluenceMapController::FindInstanceInWorld(UWorld* world)
{
	//Get all the actors in the current world:
	TArray<AActor*> actors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), actors);
	//Find the actor that has an influence map controller attached to it:
	AActor** actor = actors.FindByPredicate([](AActor*& item)
	{
		return item->FindComponentByClass<UInfluenceMapController>() != nullptr;
	});
	//Return the influence map controller component:
	return (*actor)->FindComponentByClass<UInfluenceMapController>();
}

void UInfluenceMapController::BeginPlay()
{
	Super::BeginPlay();

	targetPropagatorIndex = 0;
    InitialiseNodeNetwork();
}

void UInfluenceMapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePropagators();
	PropagateInfluences();
	DebugDraw();
}

TArray<UInfluenceMapNode*> UInfluenceMapController::GetNodes() const
{
	return nodes;
}

UInfluenceMapNodeNetwork* UInfluenceMapController::GetNodeNetwork() const
{
	return nodeNetwork;
}

void UInfluenceMapController::AddPropagator(UInfluenceMapPropagator* propagatorToAdd)
{
	propagators.Add(propagatorToAdd);
	propagatorToAdd->SetInfluenceMap(std::vector<float>(nodes.Num()));
}

void UInfluenceMapController::RemovePropagator(UInfluenceMapPropagator* propagatorToRemove)
{
	propagators.Remove(propagatorToRemove);
}

UInfluenceMapNode* UInfluenceMapController::GetClosestNode(FVector coordinates) const
{
	UInfluenceMapNode* closestNode = nullptr;
	float minimumDistance = INT_MAX;
	//for each node calculate the distance from the position to the node.
	for (UInfluenceMapNode* node : nodes)
	{
		float dist = FVector::Dist(coordinates, node->GetCoordinates());
		//if the distance is smaller than the current minimum distance, replace the closest node.
		if (dist < minimumDistance)
		{
			closestNode = node;
			minimumDistance = dist;
		}
	}
	return closestNode;
}

void UInfluenceMapController::InitialiseNodeNetwork()
{
	nodeNetwork = this->GetOwner()->FindComponentByClass<UInfluenceMapNodeNetwork>();
    nodes = nodeNetwork->CreateNetwork();
}

void UInfluenceMapController::UpdatePropagators()
{
	for (UInfluenceMapPropagator* propagator : propagators)
	{
		propagator->UpdatePropagator();
	}
}

void UInfluenceMapController::PropagateInfluences()
{
	for (UInfluenceMapPropagator* propagator : propagators)
	{
		propagator->PropagateInfluenceMap();
	}
}

void UInfluenceMapController::TargetNewPropagator(int indexOffset)
{
	int newIndex = targetPropagatorIndex + indexOffset;
	//If the new index is in the bounds of the list of propagators, update the target index:
	if (newIndex <= propagators.Num() - 1 && newIndex >= 0)
	{
		targetPropagatorIndex = newIndex;
	}
	//Else loop around to either the start or the end, as needed:
	else
	{
		targetPropagatorIndex = newIndex > propagators.Num() - 1 ? 0 : propagators.Num() - 1;
	}
}

void UInfluenceMapController::DebugDraw()
{
	if (propagators.Num() > targetPropagatorIndex)
	{
		std::vector<float> influenceMap = std::vector<float>(nodes.Num());
		FColor orange = FColor(255, 69, 0);
		FColor yellow = FColor(255, 255, 0);
		FColor blue = FColor(0, 0, 255);
		FColor cyan = FColor(0, 255, 255);

		//Get the influence map to display:
		switch (debugMapType)
		{
			case DebugMapType::Propagator:
				GetPropagatorInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::PropagatorAlly:
				GetPropagatorAllyInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::PropagatorEnemy:
				GetPropagatorEnemyInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::CompleteMap:
				GetCompleteInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::TensionMap:
				GetTensionMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::PropagatorVulnerabilityMap:
				GetVulnerabilityMap(propagators[targetPropagatorIndex], influenceMap);
				break;
			case DebugMapType::PropagatorDirectedVulnerabilityMap:
				GetDirectedVulnerabilityMap(propagators[targetPropagatorIndex], influenceMap);
				break;
		}
		NormaliseInfluenceMap(influenceMap);

		//Display all the nodes on the map, with colours representing their influence values:
		for (UInfluenceMapNode* node : nodes)
		{
			FColor color = FColor();
			if (influenceMap[node->GetIndex()] != 0.0f)
			{
				if (influenceMap[node->GetIndex()] > 0.0f)
				{
					color = FLinearColor::LerpUsingHSV(cyan, blue, influenceMap[node->GetIndex()]).ToFColor(false);
				}
				else
				{
					color = FLinearColor::LerpUsingHSV(yellow, orange, abs(influenceMap[node->GetIndex()])).ToFColor(false);
				}
				DrawDebugPoint(GetWorld(), node->GetCoordinates(), 10, color, false, 0.0f);
			}
		}
	}
}

void UInfluenceMapController::NormaliseInfluenceMap(std::vector<float>& influenceMap)
{
	//Calculate the maximum influence value in the map, taking into account negatives:
	float maxValue = *std::max_element(influenceMap.begin(), influenceMap.end());
	float minValue = *std::min_element(influenceMap.begin(), influenceMap.end());
	maxValue = abs(minValue) > maxValue ? abs(minValue) : maxValue;
	//Normalise all values in the map based on this maximum value:
	for (int i = 0; i < influenceMap.size(); i++)
	{
		if (influenceMap[i] != 0)
		{
			influenceMap[i] /= maxValue;
		}
	}
}

void UInfluenceMapController::GetPropagatorInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	influenceMap = propagator->GetInfluenceMap();
}

void UInfluenceMapController::GetPropagatorAllyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() == p->GetTeam())
		{
			std::vector<float> pInfluenceMap = p->GetInfluenceMap();
			for (int i = 0; i < influenceMap.size(); i++)
			{
				influenceMap[i] += pInfluenceMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetPropagatorEnemyInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float> &influenceMap)
{
	for (UInfluenceMapPropagator* p : propagators)
	{
		if (propagator->GetTeam() != p->GetTeam())
		{
			std::vector<float> pInfluenceMap = p->GetInfluenceMap();
			for (int i = 0; i < influenceMap.size(); i++)
			{
				influenceMap[i] += pInfluenceMap[i];
			}
		}
	}
}

void UInfluenceMapController::GetCompleteInfluenceMap(UInfluenceMapPropagator* propagator, std::vector<float> &influenceMap)
{
	std::vector<float> allyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorAllyInfluenceMap(propagator, allyInfluenceMap);
	std::vector<float> enemyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorEnemyInfluenceMap(propagator, enemyInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = allyInfluenceMap[i] - enemyInfluenceMap[i];
	}
}

void UInfluenceMapController::GetTensionMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> allyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorAllyInfluenceMap(propagator, allyInfluenceMap);
	std::vector<float> enemyInfluenceMap = std::vector<float>(influenceMap.size());
	GetPropagatorEnemyInfluenceMap(propagator, enemyInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = allyInfluenceMap[i] + enemyInfluenceMap[i];
	}
}

void UInfluenceMapController::GetVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> tensionMap = std::vector<float>(influenceMap.size());
	GetTensionMap(propagator, tensionMap);
	std::vector<float> completeInfluenceMap = std::vector<float>(influenceMap.size());
	GetCompleteInfluenceMap(propagator, completeInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = tensionMap[i] - abs(completeInfluenceMap[i]);
	}
}

void UInfluenceMapController::GetDirectedVulnerabilityMap(UInfluenceMapPropagator* propagator, std::vector<float>& influenceMap)
{
	std::vector<float> tensionMap = std::vector<float>(influenceMap.size());
	GetTensionMap(propagator, tensionMap);
	std::vector<float> completeInfluenceMap = std::vector<float>(influenceMap.size());
	GetCompleteInfluenceMap(propagator, completeInfluenceMap);
	for (int i = 0; i < influenceMap.size(); i++)
	{
		influenceMap[i] = tensionMap[i] + completeInfluenceMap[i];
	}
}

