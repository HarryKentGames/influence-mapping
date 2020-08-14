#pragma once

#include "InfluenceMapHUD.h"
#include "SlateBasics.h"
#include "SlateExtras.h"

class SInfluenceMapMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SInfluenceMapMenuWidget){}

	SLATE_ARGUMENT(TWeakObjectPtr<class AInfluenceMapHUD>, OwningHUD)

	SLATE_END_ARGS()

	TWeakObjectPtr<class AInfluenceMapHUD> OwningHUD;
	
	void Construct(const FArguments& InArgs);

	//Button Logics:
	FReply OnTargetPropagatorChanged(int indexOffset) const;
	FReply OnInfluenceMapSelected(DebugMapType mapType) const;
	FReply OnCloseMenuClicked() const;


	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	}
};
