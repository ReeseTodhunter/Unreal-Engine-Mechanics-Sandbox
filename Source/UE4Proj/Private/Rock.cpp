// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"

// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Just a static mesh that simulates physics for use with the pickup and throw codes in player

	Rock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rock"));
	Rock->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Rock->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

