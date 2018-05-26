// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CyberMageCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

ACyberMageCharacter::ACyberMageCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACyberMageCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("MeleeAttack", IE_Pressed, this, &ACyberMageCharacter::MeleeAttack);
	PlayerInputComponent->BindAction("RangeAttack", IE_Pressed, this, &ACyberMageCharacter::RangeAttack);
	PlayerInputComponent->BindAction("Spell1", IE_Pressed, this, &ACyberMageCharacter::Spell1);
	PlayerInputComponent->BindAction("Spell2", IE_Pressed, this, &ACyberMageCharacter::Spell2);
	PlayerInputComponent->BindAction("Spell3", IE_Pressed, this, &ACyberMageCharacter::Spell3);
	//PlayerInputComponent->BindAction("Spell4", IE_Pressed, this, &ACyberMageCharacter::Spell4);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACyberMageCharacter::InteractWith);
	PlayerInputComponent->BindAction("SpellList", IE_Pressed, this, &ACyberMageCharacter::SpellList);
	PlayerInputComponent->BindAction("SpellList", IE_Released, this, &ACyberMageCharacter::CloseSpellList);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACyberMageCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACyberMageCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACyberMageCharacter::TouchStopped);
}

void ACyberMageCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ACyberMageCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ACyberMageCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ACyberMageCharacter::SpellList()
{
	bShowSpells = true;
}

void ACyberMageCharacter::CloseSpellList()
{
	bShowSpells = false;
}

void ACyberMageCharacter::MeleeAttack()
{
	//Swing melee weapon
	if (!bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, "Melee Attack!");
		switch (MeleeCombo)
		{
		case CombatState::NotAttacking:
				MeleeCombo = CombatState::Combo1;
				GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Magenta, "Melee Combo 1!");
				break;
		case CombatState::Combo1:
			MeleeCombo = CombatState::Combo2;
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Magenta, "Melee Combo 2!");
			break;
		case CombatState::Combo2:
			MeleeCombo = CombatState::Combo3;
			GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Magenta, "Melee Combo 3!");
			break;
		}
	}
}

void ACyberMageCharacter::RangeAttack()
{
	if (!bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, "Range Attack!");
	}
}

void ACyberMageCharacter::Spell1()
{
	if (bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 1!");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 1!");
	}
}

void ACyberMageCharacter::Spell2()
{
	if (bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 2!");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 2!");
	}
}

void ACyberMageCharacter::Spell3()
{

	if (bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 3!");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 3!");
	}
}

void ACyberMageCharacter::Spell4()
{
	if (bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 4!");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue, "Spell Attack 4!");
	}
}

void ACyberMageCharacter::InteractWith()
{
	if (!bShowSpells)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Yellow, "Interact with object!");
	}
}