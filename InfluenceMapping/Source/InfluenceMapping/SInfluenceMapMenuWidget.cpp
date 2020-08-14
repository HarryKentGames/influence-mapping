#include "SInfluenceMapMenuWidget.h"

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
				
				//Button to select the next propagator:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnTargetPropagatorChanged, 1)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(nextPropagatorText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the previous propagator:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnTargetPropagatorChanged, -1)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(previousPropagatorText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the propagator's influence map:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::Propagator)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the propagator allies' influence maps:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::PropagatorAlly)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorAllyInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the propagator enemies' influence maps:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::PropagatorEnemy)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(propagatorEnemyInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the complete influence map:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::CompleteMap)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(completeInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the tension map:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::TensionMap)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(tensionInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the propagator's vulnerability map:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::PropagatorVulnerabilityMap)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(vulnerabilityInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to select the propagator's directed vulnerability map:
				+ SVerticalBox::Slot()
				.Padding(buttonPadding)
				[
					SNew(SButton)
					.OnClicked(this, &SInfluenceMapMenuWidget::OnInfluenceMapSelected, DebugMapType::PropagatorDirectedVulnerabilityMap)
					[
						SNew(STextBlock)
						.Font(buttonTextStyle)
						.Text(directedVulnerabilityInfluencesText)
						.Justification(ETextJustify::Center)
					]
				]

				//Button to close the menu:
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

//Button Logics:

FReply SInfluenceMapMenuWidget::OnTargetPropagatorChanged(int indexOffset) const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->TargetNewPropagator(indexOffset);
		}
	}
	return FReply::Handled();
}

FReply SInfluenceMapMenuWidget::OnInfluenceMapSelected(DebugMapType mapType) const
{
	if (OwningHUD.IsValid())
	{
		if (UInfluenceMapController* influenceMapController = OwningHUD->influenceMapController)
		{
			influenceMapController->debugMapType = mapType;
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