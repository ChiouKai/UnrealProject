// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Throw\ProjectileActor.h"
#include "MyProjectCharacter.generated.h"



UCLASS(config=Game)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()


	friend class AMyPlayerController;
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UThrowComponent> ThrowCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInteracterComponent> Interacter;


public:
	AMyProjectCharacter();

	UPROPERTY(BlueprintReadOnly)
	bool bRoll;

	bool bRollEnd;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bThrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanThrow;

	UPROPERTY(BlueprintReadOnly)
	bool bAim;

	UPROPERTY(EditAnywhere)
	FName ThrowSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectileActor> Projectile;

	FVector socketLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector cameraLocation = FVector(0, 60.0f, 70.f);

protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Reset() override;

	FTimerHandle handle;

	UFUNCTION()
	void ClearTimer(AActor* Actor, EEndPlayReason::Type EndPlayReason);

public:

	void Jump() override;

	void Roll();

	void Move(const FInputActionValue& Value);

	void NotMove(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void AIAim(bool value);

	void Aim(const FInputActionValue& Value);

	void AimTrace();

	UFUNCTION(BlueprintCallable)
	void AIThrow(AActor* target);

	void Throw();

	void InteractEnvironment();

	UFUNCTION(BlueprintCallable)
	void OnRollEnd();

	UFUNCTION(BlueprintCallable)
	void OnPreThrow();

	UFUNCTION(BlueprintCallable)
	void OnThrow();

	class UInteracterComponent* GetInteracter() const {	return Interacter; }

	class UThrowComponent* GetThrow() const { return ThrowCom; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

