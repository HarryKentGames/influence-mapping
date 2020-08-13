// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InfluenceMapAISettings.generated.h"

UENUM()
enum Behaviour
{
	Follow     UMETA(DisplayName = "Follow"),
	Flee      UMETA(DisplayName = "Flee"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INFLUENCEMAPPING_API UInfluenceMapAISettings : public UActorComponent
{
	GENERATED_BODY()

public:
	UInfluenceMapAISettings();

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Behaviour> behaviour;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
