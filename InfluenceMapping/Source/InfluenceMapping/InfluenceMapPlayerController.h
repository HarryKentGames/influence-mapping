// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InfluenceMapPlayerController.generated.h"


UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	AInfluenceMapPlayerController();

	virtual void SetupInputComponent() override;

	void OpenMenu();
};
