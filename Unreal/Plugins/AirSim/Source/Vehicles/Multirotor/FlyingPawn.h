#pragma once

#include "GameFramework/RotatingMovementComponent.h"

#include <memory>
#include "PIPCamera.h"
#include "common/common_utils/Signal.hpp"
#include "common/common_utils/UniqueValueMap.hpp"
#include "MultirotorPawnEvents.h"
#include "SimRadDetector.h"

#include "FlyingPawn.generated.h"

UCLASS()
class AIRSIM_API AFlyingPawn : public APawn
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
		float RotatorFactor = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
		bool TracePath = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
		float LifeTime = 60.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debugging")
		bool showPath = false;

	UPROPERTY()
		TMap<AActor*, float> radLevelFromSource;
		//std::map<AActor*, float> radLevelFromSource;


	//add damage event here
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	//virtual float ReceiveAnyDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
	virtual float getRadiationReading();

    AFlyingPawn();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
        FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	void getSourcesLevels();

	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out);

    //interface
    void initializeForBeginPlay();
    const common_utils::UniqueValueMap<std::string, APIPCamera*> getCameras() const;

    MultirotorPawnEvents* getPawnEvents()
    {
        return &pawn_events_;
    }
    //called by API to set rotor speed
    void setRotorSpeed(const std::vector<MultirotorPawnEvents::RotorInfo>& rotor_infos);

private: //variables
    //Unreal components
    static constexpr size_t rotor_count = 4;
    UPROPERTY() APIPCamera* camera_front_left_;
    UPROPERTY() APIPCamera* camera_front_right_;
    UPROPERTY() APIPCamera* camera_front_center_;
    UPROPERTY() APIPCamera* camera_back_center_;
    UPROPERTY() APIPCamera* camera_bottom_center_;

    UPROPERTY() URotatingMovementComponent* rotating_movements_[rotor_count];

    MultirotorPawnEvents pawn_events_;
	FVector PrevLocation;

	UPROPERTY() ASimRadDetector* rad_detector_;
	
};
