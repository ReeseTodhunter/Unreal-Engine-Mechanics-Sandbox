// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "Teleporter.h"

// Other includes
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"

// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	root->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	teleporter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Teleporter"));
	teleporter->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	teleporter->SetCollisionProfileName(TEXT("NoCollision"));

	teleporterCase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleporterCasing"));
	teleporterCase->AttachToComponent(teleporter, FAttachmentTransformRules::KeepWorldTransform);

	startLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("StartLight"));
	startLight->AttachToComponent(teleporter, FAttachmentTransformRules::KeepWorldTransform);
	startLight->Intensity = 100000.f;
	startLight->LightColor = FColor(0, 255, 255, 255);
	startLight->AttenuationRadius = 150.f;

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	trigger->AttachToComponent(teleporter, FAttachmentTransformRules::KeepWorldTransform);
	trigger->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnBoxBeginOverlap);

	exit = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Exit"));
	exit->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
	exit->SetCollisionProfileName(TEXT("NoCollision"));

	exitCase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitCase"));
	exitCase->AttachToComponent(exit, FAttachmentTransformRules::KeepWorldTransform);

	exitLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ExitLight"));
	exitLight->AttachToComponent(exit, FAttachmentTransformRules::KeepWorldTransform);
	exitLight->Intensity = 100000.f;
	exitLight->LightColor = FColor(255, 0, 0, 255);
	exitLight->AttenuationRadius = 150.f;
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Useless few lines of code more used to explore lighting originally attemping to use 2D capture and Textures to make portals this
//feature is now unused as only way I could find to create it relied on setup in the editor too heavily.

void ATeleporter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		playerCharacter->TeleportTo(exit->GetComponentLocation(), exit->GetComponentRotation(), false, true);
	}
}