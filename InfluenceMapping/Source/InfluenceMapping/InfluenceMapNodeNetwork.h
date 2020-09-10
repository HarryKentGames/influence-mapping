#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "InfluenceMapNode.h"
#include "Components/SceneComponent.h"
#include "InfluenceMapNodeNetwork.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapNodeNetwork : public USceneComponent
{
	GENERATED_BODY()

public:	
	UInfluenceMapNodeNetwork();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<UInfluenceMapNode*> CreateNetwork();
	TArray<UInfluenceMapNode*> GetNodes();
	float GetResolution();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float resolution;

	UPROPERTY(VisibleAnywhere)
	TArray<UInfluenceMapNode*> nodes;

	FVector bottomLeft;
	FVector topRight;
	float roomWidth;
	float roomLength;

	void GetRoomSize();
	void GenerateNetwork(const UNavigationSystemV1* navSys);
	void CreateMovementNetwork(const UNavigationSystemV1* navSys);
};
