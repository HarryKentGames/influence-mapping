#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceMapPropagator.h"
#include "InfluenceMapAISettings.generated.h"

UENUM()
enum Behaviour
{
	Defend     UMETA(DisplayName = "Defend"),
	Attack      UMETA(DisplayName = "Attack"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapAISettings : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<APawn*> teamMates;

	UPROPERTY(EditAnywhere)
	class AActor* flag;
	UInfluenceMapPropagator* flagPropagator;

	UInfluenceMapAISettings();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
