// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "Door.h"

// Other includes
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	doorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorMesh"));
	doorMesh->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("trigger"));
	trigger->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxBeginOverlap);
	trigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);

	innerTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("innerTrigger"));
	innerTrigger->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	innerTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxBeginOverlap);
	innerTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);

	outerTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("outerTrigger"));
	outerTrigger->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	outerTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxBeginOverlap);
	outerTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//Set up the all the required positions of the door on start
	closedRotation = doorMesh->GetComponentRotation();
	wantedRotation = closedRotation;
	openInwardRotation = closedRotation + FRotator(0.0f, 90.0f, 0.0f);
	openOutRotation = closedRotation + FRotator(0.0f, -90.0f, 0.0f);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the doors current rotation isn't the desired rotation rotate the door towards the wanted rotation
	const FRotator currentRotation = doorMesh->GetComponentRotation();
	if (!currentRotation.Equals(wantedRotation, 1.0f))
	{
		const FRotator newRotation = FMath::Lerp(currentRotation, wantedRotation, 0.02f);
		doorMesh->SetWorldRotation(newRotation);
	}
}

void ADoor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	//When overlapped by the player depending on which side of the door the player is on set the wantedRotation
	//so that the door rotates away from the player
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		if(OverlappedComp == trigger)
		{
			wantedRotation = desiredDirection;
		}
		else if (OverlappedComp == innerTrigger)
		{
			desiredDirection = openInwardRotation;
		}
		else if (OverlappedComp == outerTrigger)
		{
			desiredDirection = openOutRotation;
		}
	}
}

void ADoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//When the player leaves the trigger box close the door 
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		if(OverlappedComp == trigger)
		{
			wantedRotation = closedRotation;
		}
	}
}