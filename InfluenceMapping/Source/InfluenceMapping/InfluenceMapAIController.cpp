#include "InfluenceMapAIController.h"

void AInfluenceMapAIController::BeginPlay()
{
    Super::BeginPlay();
    AActor* actor = this->GetViewTarget();
    propagator = actor->FindComponentByClass<UInfluenceMapPropagator>();
    Super::MoveToLocation(FVector(0, 0, 0), 20.0f, true, true, true, true);
}

void AInfluenceMapAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AInfluenceMapAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);
    TArray<UInfluenceMapNode*> nodes = propagator->influenceMapController->GetNodes();
    FVector location = nodes[rand() % nodes.Num()]->GetCoordinates();
    Super::MoveToLocation(location, 20.0f, true, true, true, true);
}
