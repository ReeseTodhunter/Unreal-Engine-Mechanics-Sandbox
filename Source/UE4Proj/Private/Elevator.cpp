// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "Elevator.h"

//Other includes
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	elevator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevator"));
	elevator->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);

	target = CreateDefaultSubobject<USceneComponent>(TEXT("target"));
	target->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("trigger"));
	trigger->AttachToComponent(elevator, FAttachmentTransformRules::KeepWorldTransform);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	//Setup all the desired positions at the start
	startLocation = elevator->GetComponentLocation();
	endLocation = target->GetComponentLocation();
	wantedLocation = startLocation;
	currentLocation = startLocation;
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the elevator is not in the desired position move the elevator smoothly towards the desired position
	currentLocation = elevator->GetComponentLocation();
	if (!(currentLocation.Equals(wantedLocation, 5.0f)))
	{
		const FVector newLocation = FMath::Lerp(currentLocation, wantedLocation, 0.01f);
		elevator->SetWorldLocation(newLocation);
	}
}

void AElevator::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//When a player character steps on the elevator if it has reached it's destination set it's desired location the the opposite of it's current location
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		if(currentLocation.Equals(startLocation, 5.0f))
		{
			wantedLocation = endLocation;
		}
		else if (currentLocation.Equals(endLocation, 5.0f))
		{
			wantedLocation = startLocation;
		}
	}
}