// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UThrowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UThrowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVector projectVector;

	TArray<class USplineMeshComponent*> splineMeshArray;

	AActor* projectileEnd;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectileActor> Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BP_ProjectileEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* SplineMaterial;

	UPROPERTY()
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UFUNCTION()
	void AimTrace(FVector camPos, FVector camForward, FVector location, bool bVInterp);

	UFUNCTION()
	void StopTrace();

	void ShowSpline(struct FPredictProjectilePathResult& PredictResult);

	UFUNCTION()
	AActor* SpawnProjectile(FVector location);

	UFUNCTION()
	void SetProjectileEndHide(bool value);

private:
	FVector GetImpactPoint(FHitResult& hit, FVector startPos, FVector velocity) const;

	bool SuggestProjectileVelocity(FVector& tossVelocity,FVector location, FVector camForward, FVector impactPoint) const;

};
