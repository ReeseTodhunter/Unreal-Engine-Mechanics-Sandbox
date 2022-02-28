// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

//Forward Declaration
class UBoxComponent;
class UPointLightComponent;

UCLASS()
class UE4PROJ_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter();

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
		UStaticMeshComponent* teleporter;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* teleporterCase;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent* startLight;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* exit;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* exitCase;

	UPROPERTY(VisibleAnywhere)
		UPointLightComponent* exitLight;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* trigger;

	FVector teleportLocation;
};
