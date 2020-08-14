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
