// Fill out your copyright notice in the Description page of Project Settings.

#include "RadiationDetector.h"
#include "AirBlueprintLib.h"

// Sets default values for this component's properties
URadiationDetector::URadiationDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URadiationDetector::BeginPlay()
{
	Super::BeginPlay();
	UAirBlueprintLib::LogMessage("initiliased detected level of radiation as -1", "", LogDebugLevel::Informational, 10);
	radiationLevelCurrent = -1;
	// ...
	
}


// Called every frame
void URadiationDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

float URadiationDetector::getRadiationLevel()
{
	return radiationLevelCurrent;
}
/*
void URadiationDetector::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	radiationLevelCurrent = Damage;
}
*/



