#include "InfluenceMapAIController.h"

void AInfluenceMapAIController::BeginPlay()
{
    Super::BeginPlay();
    AActor* actor = this->GetViewTarget();
    propagator = actor->FindComponentByClass<UInfluenceMapPropagator>();
    aiSettings = actor->FindComponentByClass<UInfluenceMapAISettings>();
    atLocation = true;
}

void AInfluenceMapAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UInfluenceMapNode* currentNode = propagator->currentNode;
    if (currentNode == nullptr)
    {
        return;
    }
    std::vector<float> currentInfluenceMap = std::vector<float>(propagator->GetInfluenceMap().size());
    if (aiSettings->behaviour == Behaviour::Follow)
    {
        std::vector<int> usableIndices = std::vector<int>();
        propagator->influenceMapController->GetPropagatorEnemyInfluenceMap(propagator, currentInfluenceMap);
        if (currentNode->GetIndex() < currentInfluenceMap.size() && currentInfluenceMap[currentNode->GetIndex()] < 0.9f && atLocation)
        {
            for (std::vector<float>::iterator it = currentInfluenceMap.begin(); it != currentInfluenceMap.end(); ++it)
            {
                if (*it > 0.9f)
                {
                    usableIndices.push_back(it - currentInfluenceMap.begin());
                }
            }
            TArray<UInfluenceMapNode*> nodes = propagator->influenceMapController->GetNodes();
            FVector location = nodes[usableIndices[rand() % usableIndices.size()]]->GetCoordinates();
            Super::MoveToLocation(location, 20.0f, true, true, true, true);
            atLocation = false;
        }
    }
    else if (aiSettings->behaviour == Behaviour::Flee)
    {
        std::vector<int> usableIndices = std::vector<int>();
        propagator->influenceMapController->GetVulnerabilityMap(propagator, currentInfluenceMap);
        if (currentNode->GetIndex() < currentInfluenceMap.size() && currentInfluenceMap[currentNode->GetIndex()] > 0.0f && atLocation)
        {
            for (std::vector<float>::iterator it = currentInfluenceMap.begin(); it != currentInfluenceMap.end(); ++it)
            {
                if (*it <= 0.0f)
                {
                    usableIndices.push_back(it - currentInfluenceMap.begin());
                }
            }
            TArray<UInfluenceMapNode*> nodes = propagator->influenceMapController->GetNodes();
            FVector location = nodes[usableIndices[rand() % usableIndices.size()]]->GetCoordinates();
            Super::MoveToLocation(location, 20.0f, true, true, true, true);
            atLocation = false;
        }
    }
}

void AInfluenceMapAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    atLocation = true;
}
