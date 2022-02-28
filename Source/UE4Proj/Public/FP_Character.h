// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FP_Character.generated.h"

UCLASS()
class UE4PROJ_API AFP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool nearLadder;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* camera;

private:
	void HorizontalMove(float value);
	void VerticalMove(float value);
	void UpwardMove(float value);

	void HorizontalRotate(float value);
	void VerticalRotate(float value);

	// Function that checks if the player has jumped
	UFUNCTION()
		void CheckJump();

	//Function
	UFUNCTION()
		void CheckSprint();

	// Function that checks if the player is interacting with something
	UFUNCTION()
		void CheckInteract();

	// Function used to change the camera between first and third person
	UFUNCTION()
		void ChangeCam();

	//Casts a Ray into the scene
	UFUNCTION()
		void Ray();

	UFUNCTION()
		void SwitchClick();

	UFUNCTION()
		void Telekinesis();

	UFUNCTION()
		void Throw();

	UPROPERTY(VisibleAnywhere)
		bool firstPerson;
	
	//----------Telekinesis variables--------------
	UPROPERTY(VisibleAnywhere)
		bool holdingClick;
	UPROPERTY(VisibleAnywhere)
		AActor* target;
	//Difference between where the ray collides with an actor and the actors origin point for movement calculations
	FVector diff;
	//Distance from object when grabbed
	float dist;
	//---------------------------------------------

	UPROPERTY(VisibleAnywhere)
		bool jumping;

	UPROPERTY(VisibleAnywhere)
		bool climbing;

	UPROPERTY(VisibleAnywhere)
		bool sprinting;

	UPROPERTY()
		USpringArmComponent* arm;
};