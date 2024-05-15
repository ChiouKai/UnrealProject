// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:
	UPROPERTY()
	FTransform PlayerTransform;

	UPROPERTY()
	TArray<class AWayPoint*> EnenmyPos;

	UPROPERTY()
	TArray<FTransform> CubeTransforms;

	UPROPERTY()
	TArray<int> DeviceStates;

	UPROPERTY()
	TArray<int> SwitcherStates;


	void ClearData();
};
