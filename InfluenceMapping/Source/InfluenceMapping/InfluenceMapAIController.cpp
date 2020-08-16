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
    UInfluenceMapNode* currentNode = propagator->GetCurrentNode();
    if (currentNode == nullptr)
    {
        return;
    }
    std::vector<float> currentInfluenceMap = std::vector<float>(propagator->GetInfluenceMap().size());
    if (aiSettings->behaviour == Behaviour::Follow)
    {
        std::vector<int> usableIndices = std::vector<int>();
        propagator->GetInfluenceMapController()->GetPropagatorEnemyInfluenceMap(propagator, currentInfluenceMap);
        //If the agent is out of range of its enemy:
        if (currentNode->GetIndex() < currentInfluenceMap.size() && currentInfluenceMap[currentNode->GetIndex()] < 0.9f && atLocation)
        {
            //Find all points in range:
            for (std::vector<float>::iterator it = currentInfluenceMap.begin(); it != currentInfluenceMap.end(); ++it)
            {
                if (*it > 0.9f)
                {
                    usableIndices.push_back(it - currentInfluenceMap.begin());
                }
            }
            //Move to a random point in range:
            TArray<UInfluenceMapNode*> nodes = propagator->GetInfluenceMapController()->GetNodes();
            FVector location = nodes[usableIndices[rand() % usableIndices.size()]]->GetCoordinates();
            Super::MoveToLocation(location, 20.0f, true, true, true, true);
            atLocation = false;
        }
    }
    else if (aiSettings->behaviour == Behaviour::Flee)
    {
        std::vector<int> usableIndices = std::vector<int>();
        propagator->GetInfluenceMapController()->GetVulnerabilityMap(propagator, currentInfluenceMap);
        //If the agent is in a vulnerable space:
        if (currentNode->GetIndex() < currentInfluenceMap.size() && currentInfluenceMap[currentNode->GetIndex()] > 0.0f && atLocation)
        {
            //Find all the points which are not vulnerable:
            for (std::vector<float>::iterator it = currentInfluenceMap.begin(); it != currentInfluenceMap.end(); ++it)
            {
                if (*it <= 0.0f)
                {
                    usableIndices.push_back(it - currentInfluenceMap.begin());
                }
            }
            //Move to a random point that is not vulnerable:
            TArray<UInfluenceMapNode*> nodes = propagator->GetInfluenceMapController()->GetNodes();
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
