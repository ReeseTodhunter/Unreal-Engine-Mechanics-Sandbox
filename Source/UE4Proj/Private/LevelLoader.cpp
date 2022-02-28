// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "LevelLoader.h"

// Other includes
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ALevelLoader::ALevelLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LevelToLoad = "LVL1_HUB";

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LoadZone = CreateDefaultSubobject<USphereComponent>(TEXT("LoadZone"));
	LoadZone->AttachToComponent(Sphere, FAttachmentTransformRules::KeepWorldTransform);
	LoadZone->OnComponentBeginOverlap.AddDynamic(this, &ALevelLoader::OnBoxBeginOverlap);

	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->AttachToComponent(Sphere, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ALevelLoader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelLoader::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//When the player enters the trigger load the level to load set up in the LevelToLoad variable
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
	}
}