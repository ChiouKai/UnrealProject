// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SLWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USLWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

	//ShouldCreateSubsystem
	friend class AMyProjectGameMode;

	const char* name = "Test";

	APlayerController* playerController;
	TArray<class AAIEnemyController*> enemys;
	TArray<USceneComponent*> cubes;
	TArray<class ADevice*> devices;
	TArray<class ASwitcher*> switchers;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SaveGame();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void LoadGame();

public:

	UFUNCTION()
	bool HasSaveData();

	void AddCharacter(APlayerController* player);

	void AddEnemy(AAIEnemyController* aiController);

	UFUNCTION(BlueprintCallable)
	void AddCubes(USceneComponent* cube);

	void AddDevices(ADevice* device);

	void AddSwitchers(ASwitcher* switcher);
};
