// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

//Forward Declaration
class UBoxComponent;

UCLASS()
class UE4PROJ_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* doorMesh;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* trigger;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* innerTrigger;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* outerTrigger;

	FRotator closedRotation;
	FRotator openOutRotation;
	FRotator openInwardRotation;
	FRotator wantedRotation;
	FRotator desiredDirection;
};