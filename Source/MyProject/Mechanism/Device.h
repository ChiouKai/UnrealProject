// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StateInterface.h"
#include "Device.generated.h"

UCLASS()
class MYPROJECT_API ADevice : public AActor, public IStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADevice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int StateIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetState() override { return StateIndex; }

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchOn();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchOff();
};
