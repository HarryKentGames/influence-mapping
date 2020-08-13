#include "InfluenceMapPlayerController.h"
#include "InfluenceMapHUD.h"

AInfluenceMapPlayerController::AInfluenceMapPlayerController()
{

}

void AInfluenceMapPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AInfluenceMapPlayerController::OpenMenu);
	}
}

void AInfluenceMapPlayerController::OpenMenu()
{
	if (AInfluenceMapHUD* influenceMapHUD = Cast<AInfluenceMapHUD>(GetHUD()))
	{
		influenceMapHUD->ShowMenu();
	}
}
