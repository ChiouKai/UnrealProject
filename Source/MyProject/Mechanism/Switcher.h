// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StateInterface.h"
#include "Switcher.generated.h"

UCLASS()
class MYPROJECT_API ASwitcher : public AActor, public IStateInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitcher();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ADevice*> devices;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int StateIndex;

public:	

	int GetState() override { return StateIndex; }

	UFUNCTION(BlueprintCallable)
	void SwitchOnDevice();

	UFUNCTION(BlueprintCallable)
	void SwitchOffDevice();

};
