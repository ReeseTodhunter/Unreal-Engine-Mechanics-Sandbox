// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportBomb.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"
#include "FP_Character.h"

// Sets default values
ATeleportBomb::ATeleportBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bomb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportBomb"));
	bomb->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	hitRange = CreateDefaultSubobject<USphereComponent>(TEXT("HitRange"));
	hitRange->AttachToComponent(bomb, FAttachmentTransformRules::KeepWorldTransform);
	hitRange->OnComponentBeginOverlap.AddDynamic(this, &ATeleportBomb::OnOverlap);

	thrown = false;
}

// Called when the game starts or when spawned
void ATeleportBomb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportBomb::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//When thrown by the player when the teleport bomb hits an Actor's surface the player is teleported to the location the bomb hit and the bomb is destroyed
	if(thrown)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Teleporting player"));
		characterRef->TeleportTo(FVector(bomb->GetComponentLocation().X, bomb->GetComponentLocation().Y, bomb->GetComponentLocation().Z + 100), characterRef->GetActorRotation(), false, true);
		this->Destroy();
	}
	else //If dropped instead of thrown the bomb won't active
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Teleporter not primed"));
	}
}