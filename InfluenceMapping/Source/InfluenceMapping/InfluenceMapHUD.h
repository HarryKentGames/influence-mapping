#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InfluenceMapController.h"
#include "InfluenceMapHUD.generated.h"

UCLASS()
class INFLUENCEMAPPING_API AInfluenceMapHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UInfluenceMapController* influenceMapController;

	void ShowMenu();
	void RemoveMenu();

protected:
	TSharedPtr<class SInfluenceMapMenuWidget> MenuWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;
};
