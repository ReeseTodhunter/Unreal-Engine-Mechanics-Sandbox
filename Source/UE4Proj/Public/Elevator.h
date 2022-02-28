// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

//forward declaration
class UBoxComponent;

UCLASS()
class UE4PROJ_API AElevator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* elevator;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* target;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* trigger;

	FVector startLocation;
	FVector endLocation;
	FVector wantedLocation;
	FVector currentLocation;
};