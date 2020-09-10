#include "InfluenceMapAIAttackerController.h"

void AInfluenceMapAIAttackerController::BeginPlay()
{
    Super::BeginPlay();
    movingToFlag = false;
    movingToSafeLocation = false;
}

void AInfluenceMapAIAttackerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UInfluenceMapNode* currentNode = propagator->GetCurrentNode();
    if (currentNode == nullptr)
    {
        return;
    }
    DoAttackerLogic(currentNode);
}

void AInfluenceMapAIAttackerController::DoAttackerLogic(UInfluenceMapNode* currentNode)
{
    //Get the required maps influence maps:
    std::vector<float> totemInfluenceMap = std::vector<float>(propagator->GetInfluenceMap().size());
    propagator->GetInfluenceMapController()->GetPropagatorInfluenceMap(aiSettings->flagPropagator, totemInfluenceMap);
    std::vector<float> propagatorVulnerabilityMap = std::vector<float>(propagator->GetInfluenceMap().size());
    propagator->GetInfluenceMapController()->GetVulnerabilityMap(propagator, propagatorVulnerabilityMap, propagator->GetAlliedTeams(), propagator->GetEnemyTeams());
    std::vector<float> enemyInfluenceMap = std::vector<float>(propagator->GetInfluenceMap().size());
    propagator->GetInfluenceMapController()->GetPropagatorEnemyInfluenceMap(propagator, enemyInfluenceMap, propagator->GetEnemyTeams());

    TArray<UInfluenceMapNode*> nodes = propagator->GetInfluenceMapController()->GetNodes();
    TArray<UInfluenceMapNode*> usableNodes = TArray<UInfluenceMapNode*>();

    //If the agent is vulnerable but not fleeing, flee:
    if (currentNode->GetIndex() < propagatorVulnerabilityMap.size() && propagatorVulnerabilityMap[currentNode->GetIndex()] > 0.0f && !movingToSafeLocation)
    {
        Super::StopMovement();
        for (std::vector<float>::iterator it = enemyInfluenceMap.begin(); it != enemyInfluenceMap.end(); ++it)
        {
            if (*it <= 0.0f)
            {
                usableNodes.Add(nodes[it - enemyInfluenceMap.begin()]);
            }
        }

        if (usableNodes.Num() > 0)
        {
            FVector location = FindClosestNode(usableNodes, currentNode)->GetCoordinates();
            Super::MoveToLocation(location, 20.0f, true, true, true, true);
            movingToSafeLocation = true;
        }
    }
    //Else move towards the flag:
    else if (currentNode->GetIndex() < totemInfluenceMap.size() && totemInfluenceMap[currentNode->GetIndex()] < 0.9f && !movingToSafeLocation && !movingToFlag)
    {
        FVector location = aiSettings->flagPropagator->GetCurrentNode()->GetCoordinates();
        Super::MoveToLocation(location, 20.0f, true, true, true, true);
        movingToFlag = true;
    }
}

void AInfluenceMapAIAttackerController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    movingToFlag = false;
    movingToSafeLocation = false;
}
