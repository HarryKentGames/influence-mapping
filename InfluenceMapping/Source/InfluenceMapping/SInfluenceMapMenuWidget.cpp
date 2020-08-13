#include "SInfluenceMapMenuWidget.h"
#include "InfluenceMapHUD.h"

#define LOCTEXT_NAMESPACE "Menu"

void SInfluenceMapMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin contentPadding = FMargin(10.0f, 10.0f);
	const FMargin buttonPadding = FMargin(10.0f);

	const FText menuTitleText = LOCTEXT("MenuTitle", "Menu:");
	const FText nextPropagatorText = LOCTEXT("NextPropagator", "Next Propagator");
	const FText previousPropagatorText = LOCTEXT("PreviousPropagator", "Previous Propagator");
	const FText propagatorInfluencesText = LOCTEXT("PropagatorInfluences", "Propagator Influences");
	const FText propagatorAllyInfluencesText = LOCTEXT("AllyInfluences", "Ally Influences");
	const FText propagatorEnemyInfluencesText = LOCTEXT("EnemyInfluences", "Enemy Influences");
	const FText completeInfluencesText = LOCTEXT("CompleteInfluences", "Complete Influences");
	const FText tensionInfluencesText = LOCTEXT("TensionMap", "Tension Map");
	const FText vulnerabilityInfluencesText = LOCTEXT("VulnerabilityMap", "Propagator Vulnerabilty");
	const FText directedVulnerabilityInfluencesText = LOCTEXT("DirectedVulnerabilityMap", "Propagator Directed Vulnerability");
	const FText closeMenuText = LOCTEXT("CloseMenu", "Close Menu");

	FSlateFontInfo buttonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	buttonTextStyle.Size = 15.0f;

	FSlateFontInfo titleTextStyle = buttonTextStyle;
	titleTextStyle.Size = 20.0f;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			.Padding(contentPadding)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(titleTextStyle)
					.Text(menuTitleText)
					.Justification(ETextJustify::Center)
				]
				
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnNextPropagatorClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(nextPropagatorText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnPreviousPropagatorClicked)
				[
					SNew(STextBlock)
					.Font(buttonTextStyle)
					.Text(previousPropagatorText)
					.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnPropagatorInfluencesClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnPropagatorAllyInfluencesClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorAllyInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnPropagatorEnemyInfluencesClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorEnemyInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnCompleteInfluencesClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(completeInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnTensionMapClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(tensionInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnVulnerabilityClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(vulnerabilityInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnDirectedVulnerabilityClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(directedVulnerabilityInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnCloseMenuClicked)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(closeMenuText)
						.Justification(ETextJustify::Center)
					]
				]
			]
		];
}

FReply SInfluenceMapMenuWidget::OnNextPropagatorClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->TargetNextPropagator();
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnPreviousPropagatorClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->TargetPreviousPropagator();
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnPropagatorInfluencesClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::Propagator;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnPropagatorAllyInfluencesClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::PropagatorAlly;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnPropagatorEnemyInfluencesClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::PropagatorEnemy;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnCompleteInfluencesClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::CompleteMap;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnTensionMapClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::TensionMap;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnVulnerabilityClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::PropagatorVulnerabilityMap;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnDirectedVulnerabilityClicked() const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = DebugMapType::PropagatorDirectedVulnerabilityMap;
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnCloseMenuClicked() const
{
	if (OwningHUD.IsValid())
	{
		OwningHUD->RemoveMenu();
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE