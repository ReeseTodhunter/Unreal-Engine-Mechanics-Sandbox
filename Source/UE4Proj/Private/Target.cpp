// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "Target.h"

//other includes
#include "Components/BoxComponent.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Set the base health and move speed of the Targets
	health = 3;
	moveSpeed = 0.01f;

	Start = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	Start->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	End = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	End->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Target = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target"));
	Target->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	Trigger->AttachToComponent(Root, FAttachmentTransformRules::KeepWorldTransform);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATarget::OnBoxBeginOverlap);

	Destination = End;
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the target has reached it's destination switch it's destination so it goes back and forth between start and end
	if(Root->GetComponentLocation().Equals(Destination->GetComponentLocation(), 50.0f))
	{
		if(Destination == End)
		{
			Destination = Start;
		}
		else
		{
			Destination = End;
		}
	}
	else
	{	//if not move the Target towards it's destination
		currentLocation = Root->GetComponentLocation();
		Root->SetWorldLocation(FMath::Lerp(currentLocation, Destination->GetComponentLocation(), moveSpeed));
	}
}

void ATarget::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//When the box collider for target is overlapped decrease it's health and destroy the object overlapping it
	if(health > 1)
	{
		OtherActor->Destroy();
		health -= 1;
	}
	//if the Target has no health left destroy the target and the object overlapping it
	else
	{
		OtherActor->Destroy();
		this->Destroy();
	}
}