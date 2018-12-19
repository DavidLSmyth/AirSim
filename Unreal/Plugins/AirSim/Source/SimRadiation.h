// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimRadiation.generated.h"

UCLASS()
class AIRSIM_API ASimRadiation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimRadiation();
	UPROPERTY(Category = "Radiation", EditAnywhere)
		float baseDamage;

	UPROPERTY(Category = "Radiation", EditAnywhere)
		float outerRadius;

	UPROPERTY(Category = "Radiation", EditAnywhere)
		float innerRadius;

	UPROPERTY(Category = "Radiation", EditAnywhere)
		float dropoff = 2;

	UFUNCTION(BlueprintCallable)
	void emitRadiation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
