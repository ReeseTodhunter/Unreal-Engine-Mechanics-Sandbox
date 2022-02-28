// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelLoader.generated.h"

//Forward Declaration
class USphereComponent;
class UTextRenderComponent;

UCLASS()
class UE4PROJ_API ALevelLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		FName LevelToLoad;

private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* root;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Base;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* LoadZone;

	UPROPERTY(VisibleAnywhere)
		UTextRenderComponent* Text;
};
