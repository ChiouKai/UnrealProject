// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyProjectGameMode();

	virtual void StartPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> MainMenuTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> InteractUITemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUserWidget> PauseMenuTemplate;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ContinueGame();

	UFUNCTION()
	void ExitGame();

	UFUNCTION()
	void SetInteractUI(class UInteracterComponent* interacter) const;

	UFUNCTION()
	void ShowPauseMenu();

	UFUNCTION()
	void ClosePauseMenu();

private:

	UPROPERTY()
	class UW_MainMenu* mainMenu;
	UPROPERTY()
	class UW_Interact* interactUI;
	UPROPERTY()
	class UW_PauseMenu* pauseMenu;


	void CreateUI();

	UFUNCTION()
	void ShowMainMenu();
	
	void SpawnPlayer(const FString& IncomingName);

public:
	void RespawnPlayer(const FTransform& transform);

private:

	void SetGameInputAndView() const;

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void LoadGame();

	UFUNCTION()
	void RestartLevel();
};



