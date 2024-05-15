// Fill out your copyright notice in the Description page of Project Settings.


#include "Switcher.h"
#include "Device.h"
#include "MyProject\SLWorldSubsystem.h"


ASwitcher::ASwitcher()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASwitcher::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetSubsystem<USLWorldSubsystem>()->AddSwitchers(this);
}

void ASwitcher::SwitchOnDevice()
{
	if (devices.Num() > 0)
	{
		for (auto device : devices)
		{
			device->SwitchOn();
		}
	}
}

void ASwitcher::SwitchOffDevice()
{
	if (devices.Num() > 0)
	{
		for (auto device : devices)
		{
			device->SwitchOff();
		}
	}
}

