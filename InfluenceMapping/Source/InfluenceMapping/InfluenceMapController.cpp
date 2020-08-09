#include "InfluenceMapController.h"
#include "InfluenceMapPropagator.h"

// Sets default values for this component's properties
UInfluenceMapController::UInfluenceMapController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

    InitialiseNodeNetwork();

	FTimerHandle MyTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &UInfluenceMapController::DebugDraw, 2.0f, true, 2.0f);
}


// Called every frame
void UInfluenceMapController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdatePropagators();
	PropagateInfluences();
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

void UInfluenceMapController::DebugDraw()
{
	if (propagators.Num() > 0 && debugDraw)
	{
		FColor red = FColor(255, 0, 0);
		FColor green = FColor(0, 255, 0);
		FColor black = FColor(0, 0, 0);
		for (UInfluenceMapNode* node : nodes)
		{
			FColor color = FColor();
			color = FLinearColor::LerpUsingHSV(red, green, propagators[0]->GetInfluenceMap()[node->GetIndex()]).ToFColor(true);
			DrawDebugPoint(GetWorld(), node->GetCoordinates(), 10, color, false, 2.0f);
			for (TPair<UInfluenceMapNode*, float> neighbour : node->GetNeighbours())
			{
				float averageInfluence = (propagators[0]->GetInfluenceMap()[node->GetIndex()] + propagators[0]->GetInfluenceMap()[neighbour.Key->GetIndex()]) / 2;
				color = FLinearColor::LerpUsingHSV(red, green, averageInfluence).ToFColor(true);
				DrawDebugLine(GetWorld(), node->GetCoordinates(), neighbour.Key->GetCoordinates(), color, false, 2.0f);
			}
		}
	}
}


