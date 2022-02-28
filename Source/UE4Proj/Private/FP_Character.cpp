// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TeleportBomb.h"
#include "Rock.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bUseControllerRotationYaw = false;

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	arm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	arm->TargetArmLength = 300.0f;
	arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->AttachToComponent(arm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);

	holdingClick = false;
	firstPerson = false;
	jumping = false;
	sprinting = false;
	climbing = false;
	nearLadder = false;
}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the rayTrace has hit an actor and holdingClick is true the Telekinesis Script is called holding the target object a
	//set distance from the camera and moving it when the player moves/rotates
	if(target && holdingClick)
	{
		Telekinesis();
	}
	//While the player is not climbing(ladders) allows the player to jump
	if(!climbing)
	{
		if (jumping)
		{
			Jump();
		}
	}
	else //If the player is climbing but is no longer near a ladder sets climbing to false and returns the player's movement mode to Walking
	{
		if (!nearLadder)
		{
			climbing = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

// Called to bind functionality to input
void AFP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Horizontal", this, &AFP_Character::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &AFP_Character::VerticalMove);
	InputComponent->BindAxis("Vertical", this, &AFP_Character::UpwardMove);

	InputComponent->BindAxis("HorizontalRotation", this, &AFP_Character::HorizontalRotate);
	InputComponent->BindAxis("VerticalRotation", this, &AFP_Character::VerticalRotate);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AFP_Character::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFP_Character::CheckJump);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFP_Character::CheckSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFP_Character::CheckSprint);

	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AFP_Character::ChangeCam);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AFP_Character::CheckInteract);

	InputComponent->BindAction("Telekinesis", IE_Pressed, this, &AFP_Character::SwitchClick);
	InputComponent->BindAction("Throw", IE_Pressed, this, &AFP_Character::Throw);

	InputComponent->BindAction("CastRay", IE_Pressed, this, &AFP_Character::Ray);
}

void AFP_Character::Throw()
{
	//If the player is holding a valid object when the right mouse button is pressed sets the holdingClick bool to false
	//and casts the target object to the Rock and TeleportBomb classes. If it is a part of either class the object
	//is set to simulate physics and an impulse is added to the object. 
	if(holdingClick && target)
	{
		holdingClick = false;
		//Turns the teleport bomb's physics back on when thrown
		if(ARock* rock = Cast<ARock>(target))
		{
			rock->Rock->SetSimulatePhysics(true);
			rock->Rock->AddImpulse(camera->GetForwardVector() * 2000, NAME_None, true);
		}
		if(ATeleportBomb* bomb = Cast<ATeleportBomb>(target))
		{
			bomb->characterRef = this;	//For bombs they also take a reference of the player throwing them
			bomb->thrown = true;		//and active their thrown bool so when they hit an Actor the player will teleport
			bomb->bomb->SetSimulatePhysics(true);
			bomb->bomb->AddImpulse(camera->GetForwardVector() * 2000, NAME_None, true);
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Magenta, TEXT("Launching Object"));
		}
		target = NULL;	//Sets the target back to Null once this is done
	}
}

void AFP_Character::Telekinesis()
{
	//Gets the distance to the held object infront of the camera keeping it at that distance allows
	//the player to move the object and carry the object around by changing the object's location relative to the player camera
	FVector forward = camera->GetForwardVector();
	FVector tarLocation = target->GetActorLocation();
	FVector camLocation = camera->GetComponentLocation();

	camLocation += diff;

	//Get camera's relative location + distance infront of the cam to keep the object at
	target->SetActorRelativeLocation(FVector(camLocation.X + (forward.X * dist), camLocation.Y + (forward.Y * dist), camLocation.Z + (forward.Z * dist)));
}

void AFP_Character::SwitchClick()
{
	//Set wether or not the player is holding an object
	if(!holdingClick)
	{
		holdingClick = true;
	}
	else //if player letting go of object cast object to Rock and TeleportBomb to Turn their physics back on
	{
		holdingClick = false;
		if (ARock* rock = Cast<ARock>(target))
		{
			rock->Rock->SetSimulatePhysics(true);
		}
		if(ATeleportBomb* bombRef = Cast<ATeleportBomb>(target))
		{
			//Turns the teleport bomb's physics back on
			bombRef->bomb->SetSimulatePhysics(true);
		}
		target = NULL;
	}
}

void AFP_Character::Ray()
{
	//Cast a ray to get objects infront of the player and return a reference to them using the hitResult
	FVector start = camera->GetComponentLocation();
	FVector forward = camera->GetForwardVector();
	start = FVector(start.X + (forward.X * 100), start.Y + (forward.Y * 100), start.Z + (forward.Z * 100));
	FVector end = start + (forward * 1000);
	FHitResult hit;

	if(GetWorld())
	{
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_PhysicsBody, FCollisionQueryParams(), FCollisionResponseParams());
		//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 2, 0.f, 10.f);
		if(actorHit && hit.GetActor())
		{
			//If and actor is hit by the ray and holdingClick is true save reference to Actor as target
			if(holdingClick && (hit.GetActor() != this)) //Stops grabbing the player Actor when in third person
			{
				target = hit.GetActor();

				//Turns off the targets physics simulation if it is of class Rock or TeleportBomb
				if (ARock* rock = Cast<ARock>(target))
				{
					rock->Rock->SetSimulatePhysics(false);
				}
				if (ATeleportBomb* bomb = Cast<ATeleportBomb>(target))
				{
					bomb->bomb->SetSimulatePhysics(false);
				}
				//Get a reference to the distance from the player camera and the target to keep the target at
				//and get a reference of the difference between where the ray hit and the target's root so to correct for the distance
				dist = FVector::Distance(camera->GetComponentLocation(), target->GetActorLocation());
				diff = target->GetActorLocation() - hit.ImpactPoint;
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, hit.GetActor()->GetName());
		}
	}
}


void AFP_Character::ChangeCam()
{
	//Change where the camera is position depending on the firstPerson boolean
	if(firstPerson)
	{
		//when changing to Third person attach the camera to a USpringArmComponent so to keep the camera behind the player
		//and set the player's skeletal mesh to true
		arm->TargetArmLength = 300.f;
		arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
		camera->AttachToComponent(arm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);
		camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		camera->SetRelativeLocation(FVector(0, 0, 0));
		GetMesh()->SetVisibility(true);
		firstPerson = false;
	}
	else
	{
		//When in first person make the skeletal mesh invisible and attach the camera straight to the root component again
		GetMesh()->SetVisibility(false);
		camera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		camera->SetRelativeLocation(FVector(0, 0, 70));
		firstPerson = true;
	}
}

void AFP_Character::HorizontalMove(float value)
{
	if (value)
	{
		//While the player isn't climbing move the player side to side
		if (!climbing)
		{
			AddMovementInput(GetActorRightVector(), value);
		}
	}
}

void AFP_Character::VerticalMove(float value)
{
	if (value)
	{
		//While the player isn't climbing move the player forward and backwards
		if (!climbing)
		{
			AddMovementInput(GetActorForwardVector(), value);
		}
	}
}

void AFP_Character::UpwardMove(float value)
{
	//When the player is climbing move the player up and down
	if (value || climbing)
	{
		AddMovementInput(GetActorUpVector(), value);
	}
}

void AFP_Character::HorizontalRotate(float value)
{
	//Rotate the player left and right
	if(value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void AFP_Character::VerticalRotate(float value)
{
	if(value)
	{
		//Rotate the player cam up and down while it is within the range set here
		float temp = 0;
		if(firstPerson)
		{
			temp = camera->GetRelativeRotation().Pitch + value;
			if (temp < 90 && temp > -80)
			{
				camera->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
		else
		{
			temp = arm->GetRelativeRotation().Pitch + value;
			if (temp < 90 && temp > -80)
			{
				arm->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
	}
}

void AFP_Character::CheckInteract()
{
	//If the player is near to a ladder when E is pressed set climbing to true if it is false and change the player's movement
	//type to flying as well as increasing the Brake on flying to a large number so the player doesn't carry
	//it's momentum when a key is let go of.
	if(nearLadder)
	{
		if (!climbing)
		{
			climbing = true;
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			GetCharacterMovement()->BrakingDecelerationFlying = 100000000;
		}
		else //Lastly if the player is already climbing when pressed return the player's movement mode to walking and set climbing false
		{
			climbing = false;
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}


void AFP_Character::CheckJump()
{
	if(jumping)
	{
		jumping = false;
	}
	else
	{
		jumping = true;
	}
}

void AFP_Character::CheckSprint()
{
	if (sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = 600;
		sprinting = false;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200;
		sprinting = true;
	}
}