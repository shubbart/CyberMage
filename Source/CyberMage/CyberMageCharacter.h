// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CyberMageCharacter.generated.h"

UENUM()
enum CombatState
{
	NotAttacking,
	Combo1,
	Combo2,
	Combo3
};

UCLASS(config=Game)
class ACyberMageCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	void MeleeAttack();
	void RangeAttack();
	void Spell1();
	void Spell2();
	void Spell3();
	void Spell4();
	void InteractWith();
	void SpellList();
	void CloseSpellList();

	bool bShowSpells;

public:
	ACyberMageCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Stats")
		float Health = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Stats")
		float Energy = 40;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Character Combat State")
		int32 MeleeCombo = CombatState::NotAttacking;
};
