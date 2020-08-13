#include "InfluenceMapController.h"
#include "InfluenceMapPropagator.h"

// Sets default values for this component's properties
UInfluenceMapController::UInfluenceMapController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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


// Called when the game starts
void UInfluenceMapController::BeginPlay()
{
	Super::BeginPlay();

	targetPropagatorIndex = 0;
    InitialiseNodeNetwork();
}


// Called every frame
void UInfluenceMapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePropagators();
	PropagateInfluences();
	DebugDraw();
}

TArray<UInfluenceMapNode*> UInfluenceMapController::GetNodes()
{
	return nodes;
}

UInfluenceMapNode* UInfluenceMapController::GetClosestNode(FVector coordinates)
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

void UInfluenceMapController::AddPropagator(UInfluenceMapPropagator* propagatorToAdd)
{
	propagators.Add(propagatorToAdd);
	propagatorToAdd->SetInfluenceMap(std::vector<float>(nodes.Num()));
}

void UInfluenceMapController::RemovePropagator(UInfluenceMapPropagator* propagatorToRemove)
{
	propagators.Remove(propagatorToRemove);
}

UInfluenceMapNodeNetwork* UInfluenceMapController::GetNodeNetwork()
{
	return nodeNetwork;
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

void UInfluenceMapController::TargetNextPropagator()
{
	if (targetPropagatorIndex < propagators.Num() - 1)
	{
		targetPropagatorIndex++;
	}
	else
	{
		targetPropagatorIndex = 0;
	}
}

void UInfluenceMapController::TargetPreviousPropagator()
{
	if (targetPropagatorIndex > 0)
	{
		targetPropagatorIndex--;
	}
	else
	{
		targetPropagatorIndex = propagators.Num() - 1;
	}
}

void UInfluenceMapController::DebugDraw()
{
	if (propagators.Num() > targetPropagatorIndex)
	{
		std::vector<float> influenceMap = std::vector<float>(nodes.Num());
		if (debugMapType == DebugMapType::Propagator)
		{
			GetPropagatorInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
		}
		else if (debugMapType == DebugMapType::PropagatorAlly)
		{
			GetPropagatorAllyInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
		}
		else if (debugMapType == DebugMapType::PropagatorEnemy)
		{
			GetPropagatorEnemyInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
		}
		else if (debugMapType == DebugMapType::CompleteMap)
		{
			GetCompleteInfluenceMap(propagators[targetPropagatorIndex], influenceMap);
		}

		else if (debugMapType == DebugMapType::TensionMap)
		{
			GetTensionMap(propagators[targetPropagatorIndex], influenceMap);
		}
		else if (debugMapType == DebugMapType::PropagatorVulnerabilityMap)
		{
			GetVulnerabilityMap(propagators[targetPropagatorIndex], influenceMap);
		}
		else if (debugMapType == DebugMapType::PropagatorDirectedVulnerabilityMap)
		{
			GetDirectedVulnerabilityMap(propagators[targetPropagatorIndex], influenceMap);
		}
		NormaliseInfluenceMap(influenceMap);
		FColor red = FColor(255, 0, 0);
		FColor lightRed = FColor(255, 100, 100);
		FColor green = FColor(0, 255, 0);
		FColor lightGreen = FColor(100, 255, 100);
		for (UInfluenceMapNode* node : nodes)
		{
			FColor color = FColor();
			if (influenceMap[node->GetIndex()] != 0.0f)
			{
				if (influenceMap[node->GetIndex()] > 0.0f)
				{
					color = FLinearColor::LerpUsingHSV(lightGreen, green, influenceMap[node->GetIndex()]).ToFColor(false);
				}
				else
				{
					color = FLinearColor::LerpUsingHSV(lightRed, red, abs(influenceMap[node->GetIndex()])).ToFColor(false);
				}
				DrawDebugPoint(GetWorld(), node->GetCoordinates(), 10, color, false, 0.5f);
				/*for (TPair<UInfluenceMapNode*, float> neighbour : node->GetNeighbours())
				{
					if (influenceMap[node->GetIndex()] != 0.0f && influenceMap[neighbour.Key->GetIndex() != 0.0f])
					{
						float averageInfluence = (influenceMap[node->GetIndex()] + influenceMap[neighbour.Key->GetIndex()]) / 2;
						if (averageInfluence != 0.0f)
						{
							if (averageInfluence > 0.0f)
							{
								color = FLinearColor::LerpUsingHSV(lightGreen, green, influenceMap[node->GetIndex()]).ToFColor(false);
							}
							else
							{
								color = FLinearColor::LerpUsingHSV(lightRed, red, abs(influenceMap[node->GetIndex()])).ToFColor(false);
							}
							DrawDebugLine(GetWorld(), node->GetCoordinates(), neighbour.Key->GetCoordinates(), color, false, 0.5f);
						}
					}
				}*/
			}
		}
	}
}

void UInfluenceMapController::NormaliseInfluenceMap(std::vector<float>& influenceMap)
{
	float maxValue = *std::max_element(influenceMap.begin(), influenceMap.end());
	float minValue = *std::min_element(influenceMap.begin(), influenceMap.end());
	maxValue = abs(minValue) > maxValue ? abs(minValue) :maxValue;
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
		influenceMap[i] = tensionMap[i] + abs(completeInfluenceMap[i]);
	}
}

