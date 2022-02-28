// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportBomb.generated.h"

//Forward Declaration
class USphereComponent;
class AFP_Character;

UCLASS()
class UE4PROJ_API ATeleportBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
		bool thrown;

	AFP_Character* characterRef;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* bomb;

private:

	UPROPERTY(VisibleAnywhere)
		USphereComponent* hitRange;
};