// Fill out your copyright notice in the Description page of Project Settings.


#include "Device.h"
#include "MyProject\SLWorldSubsystem.h"

// Sets default values
ADevice::ADevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADevice::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<USLWorldSubsystem>()->AddDevices(this);
}

// Called every frame
void ADevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

