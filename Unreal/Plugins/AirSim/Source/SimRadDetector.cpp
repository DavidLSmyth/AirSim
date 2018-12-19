// Fill out your copyright notice in the Description page of Project Settings.

#include "SimRadDetector.h"
#include "AirBlueprintLib.h"
#include "Vehicles/Multirotor/FlyingPawn.h"
#include "SimRadiation.h"
#include "ConsoleManager.h"

// Sets default values
ASimRadDetector::ASimRadDetector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->bCanBeDamaged = true;
}

// Called when the game starts or when spawned
void ASimRadDetector::BeginPlay()
{
	Super::BeginPlay();
	UAirBlueprintLib::LogMessage("initiliased detected level of radiation as -1", "", LogDebugLevel::Informational, 10);
	radiationLevelCurrent = -1;
}

// Called every frame
void ASimRadDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ASimRadDetector::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UAirBlueprintLib::LogMessage("updated radiation level: ", std::to_string(DamageAmount).c_str(), LogDebugLevel::Success, 60);
	radiationLevelCurrent = DamageAmount;
	radLevelFromSource.Add(DamageCauser, DamageAmount);
	//radLevelFromSource[DamageCauser] = DamageAmount;
	//radLevelFromSource.insert_or_assign(DamageCauser, DamageAmount);
	return 0.0f;
}

/*
void ASimRadDetector::ReceiveAnyDamage(float Damage, const class UDamageType * DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	UAirBlu
	radiationLevelCurrent = Damage;
}
*/

float ASimRadDetector::getRadiationReading()
{
	float totalLevel = 0;
	/*
	int size_of_radLevelFromSource = radLevelFromSource.size();
	for (std::pair<AActor*, float> key_value: radLevelFromSource) {
		totalLevel = totalLevel + key_value.second;
	}
	*/
	for (const TPair<AActor*, float>& pair : radLevelFromSource) {
		totalLevel = totalLevel + pair.Value;
	}
	UAirBlueprintLib::LogMessageString("Returning radiation reading ", std::to_string(totalLevel), LogDebugLevel::Success, 60);
	return totalLevel;
}

/*
#include "EngineUtils.h"

template<typename T>
void FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<AActor> It(World, T::StaticClass()); It; ++It)
	{
		T* Actor = Cast<T>(*It);
		if (Actor && !Actor->IsPendingKill())
		{
			Out.Add(Actor);
		}
	}
}
*/


template<typename T>
void ASimRadDetector::FindAllActors(UWorld* World, TArray<T*>& Out)
{
	for (TActorIterator<AActor> It(World, T::StaticClass()); It; ++It)
	{
		T* Actor = Cast<T>(*It);
		if (Actor && !Actor->IsPendingKill())
		{
			Out.Add(Actor);
		}
	}
}



void ASimRadDetector::getSourcesLevels() {
	UWorld* World = GetWorld();

	UAirBlueprintLib::RunCommandOnGameThread([World, this]() {
		TArray<ASimRadiation*> rad_sources;
		FindAllActors<ASimRadiation>(World, rad_sources);
		if (rad_sources.IsValidIndex(0)) {
			UAirBlueprintLib::LogMessageString("Number of radioactive sources in scene: ", std::to_string(rad_sources.Num()), LogDebugLevel::Informational, 60);
			ASimRadiation* rad_source_;
			//each actor that can emit 'radiation' (in this case damage) does
			for (AActor* rad_source : rad_sources) {
				rad_source_ = static_cast<ASimRadiation*>(rad_source);
				rad_source_->emitRadiation();
				//trace line from me to radiation source, calculate reading
				bool reading = World->LineTraceSingleByChannel(FHitResult(), GetActorLocation(), rad_source_->GetActorLocation(), ECollisionChannel::ECC_Visibility);
				//if (!World->LineTraceSingleByChannel(FHitResult(), GetActorLocation(), rad_source_->GetActorLocation(), ECollisionChannel::ECC_Visibility)) {
					const double mean = 0;
					const double stddev = 0.1;
					std::default_random_engine generator;
					std::normal_distribution<double> dist(mean, stddev);
					float dist_sq = FVector::Dist(GetActorLocation(), rad_source_->GetActorLocation())/100 * FVector::Dist(GetActorLocation(), rad_source_->GetActorLocation()) / 100;
					float base_damage = rad_source_->baseDamage;
					float rad_reading = (base_damage/dist_sq) + dist(generator);
					radLevelFromSource.Add(rad_source, rad_reading);
				//}

			}
		}
		else {
			UAirBlueprintLib::LogMessageString("No radioactive sources found in scene ", "", LogDebugLevel::Informational, 60);
		}
	}, true);
}
