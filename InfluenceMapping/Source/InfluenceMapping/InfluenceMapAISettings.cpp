#include "InfluenceMapAISettings.h"

UInfluenceMapAISettings::UInfluenceMapAISettings()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInfluenceMapAISettings::BeginPlay()
{
	Super::BeginPlay();
	if (flag != nullptr)
	{
		flagPropagator = flag->FindComponentByClass<UInfluenceMapPropagator>();
	}	

}

void UInfluenceMapAISettings::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

