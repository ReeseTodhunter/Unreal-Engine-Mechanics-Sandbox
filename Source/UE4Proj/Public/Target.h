// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

//Forward Declaration
class UBoxComponent;

UCLASS()
class UE4PROJ_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int health;

private:

	UPROPERTY(VisibleAnywhere)
		float moveSpeed;

	UPROPERTY(VisibleAnywhere)
		FVector currentLocation;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Start;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* End;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Destination;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Target;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Trigger;

};