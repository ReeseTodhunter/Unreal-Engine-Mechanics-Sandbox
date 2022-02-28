// Fill out your copyright notice in the Description page of Project Settings.

//This classes header
#include "Ladder.h"

// Other includes
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "kismet/GameplayStatics.h"
#include "FP_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALadder::ALadder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ladder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ladder"));
	ladder->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	ladderRange = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderRange"));
	ladderRange->AttachToComponent(ladder, FAttachmentTransformRules::KeepWorldTransform);
	ladderRange->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnBoxBeginOverlap);
	ladderRange->OnComponentEndOverlap.AddDynamic(this, &ALadder::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALadder::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If the player is near the door by casting to the player class edit the player's nearLadder bool setting it to true
	AFP_Character* characterRef = Cast<AFP_Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		characterRef->nearLadder = true;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Near Ladder"));
	}
}

void ALadder::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If the player is away from the door by casting to the player class edit the player's nearLadder bool setting it to false
	AFP_Character* characterRef = Cast<AFP_Character>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter->GetUniqueID() == OtherActor->GetUniqueID())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Away From Ladder"));
		characterRef->nearLadder = false;
	}
}