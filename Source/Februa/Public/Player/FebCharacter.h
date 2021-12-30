// Copyright 2021, Choyeon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FebCharacter.generated.h"

UCLASS()
class FEBRUA_API AFebCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFebCharacter();

	FORCEINLINE float GetDeltaForwardAxis() const { return DeltaForwardAxis; }
	FORCEINLINE float GetDeltaRightAxis() const { return DeltaRightAxis; }
	FORCEINLINE float GetDeltaPitchAxis() const { return DeltaPitchAxis; }
	FORCEINLINE bool IsCrouching() const { return bCrouching; }
	FORCEINLINE bool IsSprinting() const { return bSprinting; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void MoveForward(float DeltaAxisValue) { AddMovementInput(GetActorForwardVector(), DeltaAxisValue); ServerMoveForward(DeltaAxisValue); }
	void MoveRight(float DeltaAxisValue) { AddMovementInput(GetActorRightVector(), DeltaAxisValue); ServerMoveRight(DeltaAxisValue); }
	void LookUp(float DeltaAxisValue) { AddControllerPitchInput(DeltaAxisValue); ServerLookUp(DeltaAxisValue); }

	UFUNCTION(Server, Unreliable)
	void ServerMoveForward(float DeltaAxisValue);

	UFUNCTION(Server, Unreliable)
	void ServerMoveRight(float DeltaAxisValue);

	UFUNCTION(Server, Unreliable)
	void ServerLookUp(float DeltaAxisValue);

	void Jumping() { if (bCrouching) ServerCrouching(); else Jump(); }
	void Crouching();
	void SprintingPressed();
	void SprintingReleased();

	UFUNCTION(Server, Reliable)
	void ServerCrouching();

	UFUNCTION(Server, Reliable)
	void ServerSprintingPressed();

	UFUNCTION(Server, Reliable)
	void ServerSprintingReleased();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;

	UPROPERTY(Replicated)
	float DeltaForwardAxis;

	UPROPERTY(Replicated)
	float DeltaRightAxis;

	UPROPERTY(Replicated)
	float DeltaPitchAxis;

	UPROPERTY(Replicated)
	bool bCrouching = false;

	UPROPERTY(Replicated)
	bool bSprinting = false;
};
