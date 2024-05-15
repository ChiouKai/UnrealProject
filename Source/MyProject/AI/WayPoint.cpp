// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPoint.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Components\BoxComponent.h"

// Sets default values
AWayPoint::AWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UBoxComponent* box = CreateDefaultSubobject<UBoxComponent>("Box");

	box->SetCollisionProfileName(FName("Spectator"));
	box->SetCollisionObjectType(ECC_WorldStatic);
}

void AWayPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<TEnumAsByte<EObjectTypeQuery> > objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	UClass* seekClass = AWayPoint::StaticClass();
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);

	TArray<AActor*> outResult;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), 1200.0f, objectTypes, seekClass, ignoreActors, outResult);
	TArray<AWayPoint*> NearWaypoint;
	NearWaypoint = MakeArrayView<AWayPoint*>(reinterpret_cast<AWayPoint**>(outResult.GetData()), outResult.Num());
	NearWaypointSet = TSet<AWayPoint*>(NearWaypoint);
}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();

	TArray<decltype(NearWaypointSet.begin().GetId())> test;
	for (auto i = NearWaypointSet.begin(); i != NearWaypointSet.end(); ++i)
	{
		if (!IsValid(*i))
		{
			test.Add(i.GetId());
		}
		else
		{
			(*i)->ConnectNearPoint(this);
		}
	}
	for (int i = 0; i < test.Num(); i++)
	{
		NearWaypointSet.Remove(test[i]);
	}
}

AWayPoint* AWayPoint::GetRandomPoint()
{
	int val = FMath::RandRange(0, NearWaypointSet.Num() - 1);

	auto tmp = NearWaypointSet.begin();
	for (int i = 0; i < val; i++)
	{
		++tmp;
	}
	return *tmp;
}

void AWayPoint::ConnectNearPoint(AWayPoint* point)
{
	NearWaypointSet.Add(point);
}


