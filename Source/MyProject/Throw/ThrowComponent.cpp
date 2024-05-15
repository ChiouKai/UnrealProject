// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowComponent.h"
#include "ProjectileActor.h"
#include "Kismet\GameplayStatics.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
UThrowComponent::UThrowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("ProjectileSpline"));
}


// Called when the game starts
void UThrowComponent::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	FTransform Transform;

	projectileEnd = GetWorld()->SpawnActor<AActor>(BP_ProjectileEnd, Transform, SpawnParameters);
	projectileEnd->SetActorHiddenInGame(true);
}


void UThrowComponent::AimTrace(FVector startPos, FVector forward, FVector location, bool bVInterp)
{
	FHitResult hit;
	FVector impactPoint = GetImpactPoint(hit, startPos, forward);

	FVector tossVelocity;

	SuggestProjectileVelocity(tossVelocity, location, forward, impactPoint);

	if (bVInterp)
	{
		projectVector = FMath::VInterpTo(projectVector, tossVelocity, GetWorld()->DeltaTimeSeconds, 30.0);
	}
	else
	{
		projectVector = tossVelocity;
	}

	FPredictProjectilePathParams PredictParams(2.0f, location, projectVector, 2, TraceChannel, GetOwner());
	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, PredictResult);

	ShowSpline(PredictResult);
	
	projectileEnd->SetActorHiddenInGame(false);
	projectileEnd->SetActorLocation(PredictResult.PathData.Last().Location);
	projectileEnd->SetActorRotation(PredictResult.HitResult.ImpactNormal.Rotation());
}


void UThrowComponent::StopTrace()
{
	for (auto mesh : splineMeshArray)
	{
		mesh->SetVisibility(false);
	}
	projectileEnd->SetActorHiddenInGame(true);
}

void UThrowComponent::ShowSpline(FPredictProjectilePathResult& PredictResult)
{
	Spline->ClearSplinePoints();

	for (auto result : PredictResult.PathData)
	{
		Spline->AddSplinePoint(result.Location, ESplineCoordinateSpace::Local, false);
	}
	Spline->UpdateSpline();

	int i = 0;
	for (; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent* NewMesh;

		if (i >= splineMeshArray.Num())
		{
			NewMesh = NewObject<USplineMeshComponent>(Spline);
			NewMesh->SetStaticMesh(SplineMesh);
			NewMesh->Mobility = EComponentMobility::Movable;
			NewMesh->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
			NewMesh->RegisterComponent();
			NewMesh->SetMaterial(0, SplineMaterial);
			splineMeshArray.Add(NewMesh);
		}
		else
		{
			NewMesh = splineMeshArray[i];
		}

		FVector startPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector startTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		FVector endPos = Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		FVector endTangent = Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		NewMesh->SetStartAndEnd(startPos, startTangent, endPos, endTangent, true);
		NewMesh->SetVisibility(true);
	}

	for (; i < splineMeshArray.Num(); i++)
	{
		splineMeshArray[i]->SetVisibility(false);
	}
}

AActor* UThrowComponent::SpawnProjectile(FVector location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	//SpawnParameters.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;

	FTransform Transform(projectVector.Rotation(), location + projectVector.GetSafeNormal()*2, Projectile.GetDefaultObject()->GetActorScale());
	//auto tmp = GetWorld()->SpawnActorAbsolute<AProjectileActor>(Projectile, Transform, SpawnParameters);
	AProjectileActor* tmp = GetWorld()->SpawnActor<AProjectileActor>(Projectile, location + projectVector.GetSafeNormal() * 2, projectVector.Rotation());
	tmp->MeshComp->IgnoreActorWhenMoving(GetOwner(), true);
	return tmp;
}

void UThrowComponent::SetProjectileEndHide(bool value)
{
	projectileEnd->SetActorHiddenInGame(value);
}


FVector UThrowComponent::GetImpactPoint(FHitResult& hit, FVector startPos, FVector forward) const
{
	FVector impactPoint = startPos + forward * 5000.0;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(hit, startPos, impactPoint, ECC_Camera, CollisionParameters);

	return hit.bBlockingHit ? hit.ImpactPoint : impactPoint;
}

bool UThrowComponent::SuggestProjectileVelocity(FVector& tossVelocity, FVector location, FVector camForward, FVector impactPoint) const
{
	float tossSpeed = Projectile.GetDefaultObject()->GetInitialSpeed();
	tossVelocity = tossSpeed * camForward;
	FCollisionResponseParams ResponseParams;
	TArray<AActor*> ignore;
	ignore.Add(GetOwner());

	bool isVaild = UGameplayStatics::SuggestProjectileVelocity(GetWorld(), tossVelocity, location, impactPoint, tossSpeed, false, 0.0f, 0.0f
		, ESuggestProjVelocityTraceOption::DoNotTrace, ResponseParams, ignore);

	return isVaild;
}