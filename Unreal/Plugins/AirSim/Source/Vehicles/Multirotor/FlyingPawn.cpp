#include "FlyingPawn.h"
#include "Components/StaticMeshComponent.h"
#include "AirBlueprintLib.h"
#include "common/CommonStructs.hpp"
#include "common/Common.hpp"
#include "DrawDebugHelpers.h"
#include "SimRadDetector.h"

AFlyingPawn::AFlyingPawn()
{
    pawn_events_.getActuatorSignal().connect_member(this, &AFlyingPawn::setRotorSpeed);
	this->bCanBeDamaged = true;
}

void AFlyingPawn::BeginPlay()
{
    Super::BeginPlay();

    for (auto i = 0; i < rotor_count; ++i) {
        rotating_movements_[i] = UAirBlueprintLib::GetActorComponent<URotatingMovementComponent>(this, TEXT("Rotation") + FString::FromInt(i));
    }
	PrevLocation = this->GetActorLocation();
}

void AFlyingPawn::initializeForBeginPlay()
{    
    //get references of existing camera
    camera_front_right_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontRightCamera")))->GetChildActor());
    camera_front_left_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontLeftCamera")))->GetChildActor());
    camera_front_center_ = Cast<APIPCamera>(
        (UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("FrontCenterCamera")))->GetChildActor());
    camera_back_center_ = Cast<APIPCamera>(
(UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("BackCenterCamera")))->GetChildActor());
camera_bottom_center_ = Cast<APIPCamera>(
(UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("BottomCenterCamera")))->GetChildActor());

//rad_detector_ = Cast<ASimRadDetector>((UAirBlueprintLib::GetActorComponent<UChildActorComponent>(this, TEXT("rad_detector")))->GetChildActor());
}

void AFlyingPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	pawn_events_.getPawnTickSignal().emit(DeltaSeconds);

	//make this optional
	if (PrevLocation != this->GetActorLocation() && showPath) {
		DrawDebugLine(this->GetWorld(), PrevLocation, this->GetActorLocation(), FColor::Red, false, LifeTime, uint8(0), 25);
		PrevLocation = this->GetActorLocation();
	}
}


void AFlyingPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	camera_front_right_ = nullptr;
	camera_front_left_ = nullptr;
	camera_front_center_ = nullptr;
	camera_back_center_ = nullptr;
	camera_bottom_center_ = nullptr;

	Super::EndPlay(EndPlayReason);
}

const common_utils::UniqueValueMap<std::string, APIPCamera*> AFlyingPawn::getCameras() const
{
	common_utils::UniqueValueMap<std::string, APIPCamera*> cameras;
	cameras.insert_or_assign("front_center", camera_front_center_);
	cameras.insert_or_assign("front_right", camera_front_right_);
	cameras.insert_or_assign("front_left", camera_front_left_);
	cameras.insert_or_assign("bottom_center", camera_bottom_center_);
	cameras.insert_or_assign("back_center", camera_back_center_);

	cameras.insert_or_assign("0", camera_front_center_);
	cameras.insert_or_assign("1", camera_front_right_);
	cameras.insert_or_assign("2", camera_front_left_);
	cameras.insert_or_assign("3", camera_bottom_center_);
	cameras.insert_or_assign("4", camera_back_center_);

	cameras.insert_or_assign("", camera_front_center_);
	cameras.insert_or_assign("fpv", camera_front_center_);

	return cameras;
}

void AFlyingPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	pawn_events_.getCollisionSignal().emit(MyComp, Other, OtherComp, bSelfMoved, HitLocation,
		HitNormal, NormalImpulse, Hit);
}

void AFlyingPawn::setRotorSpeed(const std::vector<MultirotorPawnEvents::RotorInfo>& rotor_infos)
{
	for (auto rotor_index = 0; rotor_index < rotor_infos.size(); ++rotor_index) {
		auto comp = rotating_movements_[rotor_index];
		if (comp != nullptr) {
			comp->RotationRate.Yaw =
				rotor_infos.at(rotor_index).rotor_speed * rotor_infos.at(rotor_index).rotor_direction *
				180.0f / M_PIf * RotatorFactor;
		}
	}
}
/*
float AFlyingPawn::ReceiveAnyDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UAirBlueprintLib::LogMessage("updated radiation level: ", std::to_string(DamageAmount).c_str(), LogDebugLevel::Success, 60);
	//radiationLevelCurrent = DamageAmount;
	//radLevelFromSource.Add(DamageCauser, DamageAmount);
	radLevelFromSource[DamageCauser] = DamageAmount;
	return 0;
}
*/

float AFlyingPawn::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) {
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UAirBlueprintLib::LogMessage("updated radiation level: ", std::to_string(DamageAmount).c_str(), LogDebugLevel::Success, 60);
	//radiationLevelCurrent = DamageAmount;
	radLevelFromSource.Add(DamageCauser, DamageAmount);
	//radLevelFromSource[DamageCauser] = DamageAmount;
	return 0;
}


float AFlyingPawn::getRadiationReading()
{
	float totalLevel = 0;
	int size_of_radLevelFromSource = radLevelFromSource.Num();
	/*
	for (std::pair<AActor*, float> key_value : radLevelFromSource) {
		totalLevel = totalLevel + key_value.second;
	}
	*/
	for (const TPair<AActor*, float>& pair : radLevelFromSource){
		totalLevel = totalLevel + pair.Value;
	}
	UAirBlueprintLib::LogMessageString("Returning radiation reading ", std::to_string(totalLevel), LogDebugLevel::Success, 60);
	return totalLevel;
}

template<typename T>
void AFlyingPawn::FindAllActors(UWorld* World, TArray<T*>& Out)
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


void AFlyingPawn::getSourcesLevels() {
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
				const double mean = 0.1;
				const double stddev = 0.1;
				std::default_random_engine generator;
				std::normal_distribution<double> dist(mean, stddev);
				float dist_sq = FVector::Dist(GetActorLocation(), rad_source_->GetActorLocation()) / 100 * FVector::Dist(GetActorLocation(), rad_source_->GetActorLocation()) / 100;
				float base_damage = rad_source_->baseDamage;
				float rad_reading = (base_damage / dist_sq) + dist(generator);
				radLevelFromSource.Add(rad_source, rad_reading);
				//}

			}
		}
		else {
			UAirBlueprintLib::LogMessageString("No radioactive sources found in scene ", "", LogDebugLevel::Informational, 60);
		}
	}, true);
}
