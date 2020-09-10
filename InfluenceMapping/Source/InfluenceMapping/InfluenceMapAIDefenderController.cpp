#include "InfluenceMapAIDefenderController.h"

void AInfluenceMapAIDefenderController::BeginPlay()
{
    Super::BeginPlay();
    movingToInterceptAttacker = false;
    movingToPatrolLocation = false;
}

void AInfluenceMapAIDefenderController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UInfluenceMapNode* currentNode = propagator->GetCurrentNode();
    if (currentNode == nullptr)
    {
        return;
    }
    DoDefenderLogic(currentNode);
}

void AInfluenceMapAIDefenderController::DoDefenderLogic(UInfluenceMapNode* currentNode)
{
    //Get destinations that teammates are going to:
    TMap<UInfluenceMapPropagator*, UInfluenceMapNode*> teamMateDestinations = TMap<UInfluenceMapPropagator*, UInfluenceMapNode*>();
    for (AInfluenceMapAIController* teamMateController : teamMateControllers)
    {
        teamMateDestinations.Add(teamMateController->propagator, teamMateController->locationNode);
    }

    std::vector<float> totemTensionMap = std::vector<float>(propagator->GetInfluenceMap().size());
    propagator->GetInfluenceMapController()->GetTensionMap(aiSettings->flagPropagator, totemTensionMap, aiSettings->flagPropagator->GetAlliedTeams(), aiSettings->flagPropagator->GetEnemyTeams());
    std::vector<float> normalisedTensionMap = totemTensionMap;
    propagator->GetInfluenceMapController()->NormaliseInfluenceMap(normalisedTensionMap);

    bool mapHasTension = false;

    TArray<UInfluenceMapNode*> nodes = propagator->GetInfluenceMapController()->GetNodes();
    TArray<UInfluenceMapNode*> nodesWithTension = TArray<UInfluenceMapNode*>();

    //Get all the nodes that have sufficient tension that a teammate is not dealing with already:
    TArray<UInfluenceMapNode*> claimedNodes;
    teamMateDestinations.GenerateValueArray(claimedNodes);
    for (std::vector<float>::iterator it = totemTensionMap.begin(); it != totemTensionMap.end(); ++it)
    {
        if (*it >= 0.5f && !claimedNodes.Contains(nodes[it - totemTensionMap.begin()]) && !IsTeamMateDealingWithNearbyNode(nodes[it - totemTensionMap.begin()], teamMateDestinations))
        {
            mapHasTension = true;
            break;
        }
    }

    //If there is undealt with tension, get all nodes with tension above a threshold:
    if (mapHasTension)
    {
        for (std::vector<float>::iterator it = normalisedTensionMap.begin(); it != normalisedTensionMap.end(); ++it)
        {
            if (*it >= 1.0f && !claimedNodes.Contains(nodes[it - normalisedTensionMap.begin()]) && !IsTeamMateDealingWithNearbyNode(nodes[it - normalisedTensionMap.begin()], teamMateDestinations))
            {
                nodesWithTension.Add(nodes[it - normalisedTensionMap.begin()]);
            }
        }
    }

    //Interupt movement if patrolling and there is now tension, or if intercepting to location that no longer has tension:
    if (locationNode != nullptr && locationNode->GetIndex() < totemTensionMap.size() && ((movingToPatrolLocation && nodesWithTension.Num() > 0) || movingToInterceptAttacker && normalisedTensionMap[locationNode->GetIndex()] < 0.8f))
    {
        Super::StopMovement();
        movingToPatrolLocation = false;
        movingToInterceptAttacker = false;
    }

    //If there is no tension, patrol:
    if (nodesWithTension.Num() <= 0 && !movingToPatrolLocation)
    {
        std::vector<float> flagInfluenceMap = std::vector<float>(propagator->GetInfluenceMap().size());
        //Get all nodes influenced by the flag:
        propagator->GetInfluenceMapController()->GetPropagatorInfluenceMap(aiSettings->flagPropagator, flagInfluenceMap);
        TArray<UInfluenceMapNode*> nodesWithInfluence = TArray<UInfluenceMapNode*>();
        for (std::vector<float>::iterator it = flagInfluenceMap.begin(); it != flagInfluenceMap.end(); ++it)
        {
            if (*it > 0.5f)
            {
                nodesWithInfluence.Add(nodes[it - flagInfluenceMap.begin()]);
            }
        }
        //Move to a node influenced by the flag:
        if (nodesWithInfluence.Num() > 0)
        {
            locationNode = nodesWithInfluence[rand() % nodesWithInfluence.Num()];
            FVector location = locationNode->GetCoordinates();
            Super::MoveToLocation(location, 20.0f, true, true, true, true);
            movingToPatrolLocation = true;
        }
    }

    //Else move to intercept:
    else if(nodesWithTension.Num() > 0 && !movingToInterceptAttacker)
    {
        locationNode = FindClosestNode(nodesWithTension, currentNode);
        FVector location = locationNode->GetCoordinates();
        Super::MoveToLocation(location, 20.0f, true, true, true, true);
        movingToInterceptAttacker = true;
    }
}

void AInfluenceMapAIDefenderController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    movingToInterceptAttacker = false;
    movingToPatrolLocation = false;
}
