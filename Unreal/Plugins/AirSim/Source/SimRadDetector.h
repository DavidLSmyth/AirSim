// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>
#include "GameFramework/Actor.h"
#include "SimRadiation.h"
#include "SimRadDetector.generated.h"


UCLASS()
class AIRSIM_API ASimRadDetector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimRadDetector();
	float radiationLevelCurrent;

	UPROPERTY()
		TMap<AActor*, float> radLevelFromSource;

	//std::map<AActor*, float> radLevelFromSource;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	//virtual void ReceiveAnyDamage(float Damage, const class UDamageType * DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;
	float getRadiationReading();
	
	void getSourcesLevels();

	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out);
	
	
};
