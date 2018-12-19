// Fill out your copyright notice in the Description page of Project Settings.

#include "SimRadiation.h"
#include "AirBlueprintLib.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASimRadiation::ASimRadiation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimRadiation::BeginPlay()
{
	Super::BeginPlay();
	UAirBlueprintLib::LogMessage("Drawing debug sphere centered at", this->GetActorLocation().ToString(), LogDebugLevel::Informational, 20);
	UAirBlueprintLib::LogMessage(" with radius ", UTF8_TO_TCHAR(std::to_string(outerRadius).c_str()), LogDebugLevel::Informational, 20);
	DrawDebugSphere(this->GetWorld(), this->GetActorLocation(), outerRadius, 12, FColor::Purple, true, 200, (uint8)'\000', 20);
	DrawDebugSphere(this->GetWorld(), this->GetActorLocation(), innerRadius, 12, FColor::Purple, true, 200, (uint8)'\000', 20);
}


void ASimRadiation::emitRadiation() {
	UAirBlueprintLib::LogMessageString("SimRadiation emitting radiation", "", LogDebugLevel::Success,10);
	bool appliedDamage = UGameplayStatics::ApplyRadialDamageWithFalloff(this, baseDamage, 0, this->GetActorLocation(), innerRadius, outerRadius, dropoff, UDamageType::StaticClass(), TArray<AActor*>(), this, this->GetInstigatorController(),ECollisionChannel::ECC_Visibility);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, baseDamage, 0, this->GetActorLocation(), innerRadius, outerRadius, dropoff, UDamageType::StaticClass(), TArray<AActor*>(), this, this->GetInstigatorController(), ECollisionChannel::ECC_PhysicsBody);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, baseDamage, 0, this->GetActorLocation(), innerRadius, outerRadius, dropoff, UDamageType::StaticClass(), TArray<AActor*>(), this, this->GetInstigatorController(), ECollisionChannel::ECC_WorldDynamic);
	//UGameplayStatics::ApplyRadialDamageWithFalloff(this, baseDamage, 0, this->GetActorLocation(), innerRadius, outerRadius, dropoff, UDamageType::StaticClass(), TArray<AActor*>(), this, this->GetInstigatorController(), ECollisionChannel::ECC_WorldStatic);
	//UAirBlueprintLib::LogMessageString("Successfully emitted radiation:", std::to_string(appliedDamage), LogDebugLevel::Success, 10);
}

// Called every frame
void ASimRadiation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

