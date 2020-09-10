#include "InfluenceMapAIController.h"


void AInfluenceMapAIController::BeginPlay()
{
    Super::BeginPlay();
    actor = this->GetViewTarget();
    propagator = actor->FindComponentByClass<UInfluenceMapPropagator>();
    aiSettings = actor->FindComponentByClass<UInfluenceMapAISettings>();

    for (APawn* teamMate : aiSettings->teamMates)
    {
        teamMateControllers.Add(Cast<AInfluenceMapAIController>(teamMate->GetController()));
    }
}

UInfluenceMapNode* AInfluenceMapAIController::FindClosestNode(TArray<UInfluenceMapNode*> nodes, UInfluenceMapNode* currentNode)
{
    UInfluenceMapNode* closestNode = nodes[0];
    float smallestDistance = INT_MAX;
    for (UInfluenceMapNode* node : nodes)
    {
        if (node == currentNode)
        {
            continue;
        }
        float distance = FVector::Dist(node->GetCoordinates(), currentNode->GetCoordinates());
        if (distance < smallestDistance)
        {
            smallestDistance = distance;
            closestNode = node;
        }
    }
    return closestNode;
}

bool AInfluenceMapAIController::IsTeamMateDealingWithNearbyNode(UInfluenceMapNode* node, TMap<UInfluenceMapPropagator*, UInfluenceMapNode*> teamMateDestinations)
{
    for (TPair<UInfluenceMapPropagator*, UInfluenceMapNode*> teamMateDestination : teamMateDestinations)
    {
        if (FVector::Dist(teamMateDestination.Value->GetCoordinates(), node->GetCoordinates()) < teamMateDestination.Key->GetInfluenceRange() / 2.0f)
        {
            return true;
        }
    }
    return false;
}

