// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

class SInfluenceMapMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SInfluenceMapMenuWidget){}

	SLATE_ARGUMENT(TWeakObjectPtr<class AInfluenceMapHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnNextPropagatorClicked() const;
	FReply OnPreviousPropagatorClicked() const;
	FReply OnPropagatorInfluencesClicked() const;
	FReply OnPropagatorAllyInfluencesClicked() const;
	FReply OnPropagatorEnemyInfluencesClicked() const;
	FReply OnCompleteInfluencesClicked() const;
	FReply OnTensionMapClicked() const;
	FReply OnVulnerabilityClicked() const;
	FReply OnDirectedVulnerabilityClicked() const;
	FReply OnCloseMenuClicked() const;

	TWeakObjectPtr<class AInfluenceMapHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override
	{
		return true;
	}
};
