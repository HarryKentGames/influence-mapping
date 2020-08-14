#include "InfluenceMappingGameModeBase.h"
#include "InfluenceMapHUD.h"
#include "InfluenceMapPlayerController.h"


AInfluenceMappingGameModeBase::AInfluenceMappingGameModeBase()
{
	HUDClass = AInfluenceMapHUD::StaticClass();
	PlayerControllerClass = AInfluenceMapPlayerController::StaticClass();
};

void AInfluenceMappingGameModeBase::StartPlay()
{
	Super::StartPlay();
}
