// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Throw/ThrowComponent.h"
#include "Mechanism/InteracterComponent.h"


//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;

	ThrowCom = CreateDefaultSubobject<UThrowComponent>(TEXT("ThrowComponent"));
	AddOwnedComponent(ThrowCom);
	Interacter = CreateDefaultSubobject<UInteracterComponent>(TEXT("InteracterComponent"));
}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	bRollEnd = true;

	Interacter->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	
	OnEndPlay.AddDynamic(this, &AMyProjectCharacter::ClearTimer);
}

void AMyProjectCharacter::Reset()
{
	bRollEnd = true;
	bThrow = false;
	bAim = false;
	bRollEnd = false;
	bCanThrow = false;
	ThrowCom->StopTrace();
}

void AMyProjectCharacter::ClearTimer(AActor* Actor, EEndPlayReason::Type EndPlayReason )
{
	GetWorld()->GetTimerManager().ClearTimer(handle);
}


void AMyProjectCharacter::Jump()
{
	if (!bAim)
	{
		Super::Jump();
	}
}

void AMyProjectCharacter::Roll()
{
	if(!bAim && bRollEnd && !GetMovementComponent()->IsFalling())
	{
		GetCharacterMovement()->MaxWalkSpeed = 650.0f;
		bRoll = true;
		bRollEnd = false;
	}
}

void AMyProjectCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMyProjectCharacter::NotMove(const FInputActionValue& Value)
{
	if (bRoll)
	{
		AddMovementInput(GetActorForwardVector(), 1.0);
	}
}

void AMyProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (bAim)
	{
		FRotator ControlRotation = GetControlRotation();
		double Pitch = ControlRotation.Pitch > 180.0 ? ControlRotation.Pitch - 360.0 : ControlRotation.Pitch;

		if (Pitch > 45.0 && LookAxisVector.Y < 0.0 || Pitch < -45.0 && LookAxisVector.Y > 0.0)
		{
			LookAxisVector.Y = 0;
		}
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyProjectCharacter::AIAim(bool value)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bAim = value;
		bCanThrow = false;
	}
}

void AMyProjectCharacter::Aim(const FInputActionValue& Value)
{
	if (!GetCharacterMovement()->IsFalling())
	{
		bAim = Value.Get<bool>();

		if (bAim)
		{
			bCanThrow = false;
			CameraBoom->TargetArmLength = 75.0f;
			CameraBoom->SocketOffset = cameraLocation;
		}
		else
		{
			CameraBoom->TargetArmLength = 400.0f;
			CameraBoom->SocketOffset = FVector::ZeroVector;

			ThrowCom->StopTrace();
		}

		GetCharacterMovement()->bUseControllerDesiredRotation = bAim;
		GetCharacterMovement()->bOrientRotationToMovement = !bAim;
	}
}

void AMyProjectCharacter::AimTrace()
{
	if (bCanThrow)
	{
		socketLocation = GetMesh()->GetSocketLocation(ThrowSocketName);
		ThrowCom->AimTrace(FollowCamera->GetComponentLocation(), FollowCamera->GetForwardVector(), socketLocation, true);
	}
}

void AMyProjectCharacter::AIThrow(AActor* target)
{
	if (bAim && bCanThrow)
	{
		bCanThrow = false;
		bThrow = true;

		socketLocation = GetMesh()->GetSocketLocation(ThrowSocketName);
		FVector socketToTarget = (target->GetActorLocation() - socketLocation).GetSafeNormal();
		ThrowCom->AimTrace(socketLocation, socketToTarget, socketLocation, false);

		AActor* projectile = ThrowCom->SpawnProjectile(socketLocation);

		MoveIgnoreActorAdd(projectile);
		GetCapsuleComponent()->IgnoreActorWhenMoving(projectile, true);
		GetMesh()->IgnoreActorWhenMoving(projectile, true);

		GetWorld()->GetTimerManager().SetTimer(handle, [=]{ ThrowCom->StopTrace(); }, 1.0, false);
	}
}
void AMyProjectCharacter::Throw()
{
	if (bAim && bCanThrow)
	{
		bCanThrow = false;
		bThrow = true;
		ThrowCom->StopTrace();

		AActor* projectile = ThrowCom->SpawnProjectile(socketLocation);

		MoveIgnoreActorAdd(projectile);
		GetCapsuleComponent()->IgnoreActorWhenMoving(projectile, true);
		GetMesh()->IgnoreActorWhenMoving(projectile, true);
	}
}

void AMyProjectCharacter::InteractEnvironment()
{
	Interacter->Interact();
}

void AMyProjectCharacter::OnRollEnd()
{
	bRollEnd = true;
	bRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void AMyProjectCharacter::OnPreThrow()
{
	bCanThrow = true;
}

void AMyProjectCharacter::OnThrow()
{
	bThrow = false;
}


