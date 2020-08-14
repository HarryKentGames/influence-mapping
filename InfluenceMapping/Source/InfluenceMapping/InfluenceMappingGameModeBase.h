#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfluenceMapHUD.h"
#include "InfluenceMappingGameModeBase.generated.h"

UCLASS()
class INFLUENCEMAPPING_API AInfluenceMappingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInfluenceMappingGameModeBase();
	virtual void StartPlay() override;
};
