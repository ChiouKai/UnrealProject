// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayPoint.generated.h"

UCLASS()
class MYPROJECT_API AWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWayPoint();

protected:

	void OnConstruction(const FTransform& Transform) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere)
	TSet<AWayPoint*> NearWaypointSet;

	UFUNCTION(BlueprintCallable)
	AWayPoint* GetRandomPoint();

private:

	void ConnectNearPoint(AWayPoint* point);
};
