// Copyright 2021, Choyeon. All Rights Reserved.

#include "Player/FebCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AFebCharacter::AFebCharacter()
	: Camera(CreateDefaultSubobject<UCameraComponent>(TEXT("Camera")))
{
	GetCapsuleComponent()->InitCapsuleSize(22.f, 92.f);

	GetMesh()->SetRelativeLocation(FVector(4.f, 0.f, -92.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetMesh(), TEXT("SOCKET_Camera"));
	Camera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void AFebCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFebCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFebCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFebCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFebCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFebCharacter::Jumping);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AFebCharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AFebCharacter::ServerCrouching);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AFebCharacter::ServerSprintingPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AFebCharacter::ServerSprintingReleased);
}

void AFebCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFebCharacter, DeltaForwardAxis);
	DOREPLIFETIME(AFebCharacter, DeltaRightAxis);
	DOREPLIFETIME(AFebCharacter, DeltaPitchAxis);
	DOREPLIFETIME(AFebCharacter, bCrouching);
	DOREPLIFETIME(AFebCharacter, bSprinting);
}

void AFebCharacter::ServerMoveForward_Implementation(float DeltaAxisValue)
{
	DeltaForwardAxis = DeltaAxisValue;
}

void AFebCharacter::ServerMoveRight_Implementation(float DeltaAxisValue)
{
	DeltaRightAxis = DeltaAxisValue;
}

void AFebCharacter::ServerLookUp_Implementation(float DeltaAxisValue)
{
	DeltaPitchAxis = DeltaAxisValue;
}

void AFebCharacter::Crouching()
{
	GetCharacterMovement()->MaxWalkSpeed = bCrouching ? 300.f : 150.f;
	ServerCrouching();
}

void AFebCharacter::SprintingPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	ServerSprintingPressed();
}

void AFebCharacter::SprintingReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	ServerSprintingReleased();
}

void AFebCharacter::ServerCrouching_Implementation()
{
	bCrouching = !bCrouching;

	if (bCrouching)
		bSprinting = false;
}

void AFebCharacter::ServerSprintingPressed_Implementation()
{
	bCrouching = false;
	bSprinting = true;
}

void AFebCharacter::ServerSprintingReleased_Implementation()
{
	bSprinting = false;
}
