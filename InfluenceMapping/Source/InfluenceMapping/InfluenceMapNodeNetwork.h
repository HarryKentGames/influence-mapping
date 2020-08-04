// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this component's properties
	UInfluenceMapNodeNetwork();

private:
	UPROPERTY(EditAnywhere)
	float resolution;

	UPROPERTY(EditAnywhere)
	bool debugDraw;

	TArray<UInfluenceMapNode*> nodes;

	FVector bottomLeft;
	FVector topRight;
	float roomWidth;
	float roomLength;

	void GetRoomSize();
	void GenerateNetwork(const UNavigationSystemV1* navSys);
	void CreateMovementNetwork(const UNavigationSystemV1* navSys);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
