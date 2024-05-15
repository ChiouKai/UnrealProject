// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectGameMode.h"
#include "MyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/W_MainMenu.h"
#include "UI/W_Interact.h"
#include "UI/W_PauseMenu.h"
#include "SLWorldSubsystem.h"
#include "Mechanism/InteracterComponent.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	DefaultPawnClass = PlayerPawnBPClass.Class;
}

void AMyProjectGameMode::StartPlay()
{
	Super::StartPlay();
	CreateUI();
	ShowMainMenu();
	GetWorld()->DestroyActor(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
}

void AMyProjectGameMode::StartGame()
{
	SpawnPlayer("1");
	SetGameInputAndView();
	mainMenu->RemoveFromParent();
}

void AMyProjectGameMode::ContinueGame()
{
	GetWorld()->GetSubsystem<USLWorldSubsystem>()->LoadGame();
	SetGameInputAndView();
	mainMenu->RemoveFromParent();
}

void AMyProjectGameMode::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void AMyProjectGameMode::SetInteractUI(UInteracterComponent* interacter) const
{
	if (interactUI)
	{
		interacter->CanInteract.AddUObject(interactUI, &UW_Interact::ShowText);
		interacter->CanNotInteract.AddUObject(interactUI, &UW_Interact::CloseText);
	}
}


void AMyProjectGameMode::CreateUI()
{
	if (MainMenuTemplate)
	{
		mainMenu = CreateWidget<UW_MainMenu>(GetWorld(), MainMenuTemplate);

		mainMenu->StartButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::StartGame);
		mainMenu->ContinueButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::ContinueGame);
		mainMenu->ExitButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::ExitGame);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("mainMenu"));
	}

	if (InteractUITemplate)
	{
		interactUI = CreateWidget<UW_Interact>(GetWorld(), InteractUITemplate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("interactUI"));
	}

	if (PauseMenuTemplate)
	{
		pauseMenu = CreateWidget<UW_PauseMenu>(GetWorld(), PauseMenuTemplate);
		pauseMenu->ResumeButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::ClosePauseMenu);
		pauseMenu->SaveButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::SaveGame);
		pauseMenu->LoadButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::LoadGame);
		pauseMenu->MainMenuButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::RestartLevel);
		pauseMenu->ExitButton->OnClicked.AddDynamic(this, &AMyProjectGameMode::ExitGame);
	}
}

void AMyProjectGameMode::ShowMainMenu()
{
	mainMenu->AddToViewport();
	bool hasSaveData = GetWorld()->GetSubsystem<USLWorldSubsystem>()->HasSaveData();
	mainMenu->SetContinueButtonEnabled(hasSaveData);

	FInputModeUIOnly mode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(mode);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}

void AMyProjectGameMode::ShowPauseMenu()
{
	pauseMenu->AddToViewport();
	bool hasSaveData = GetWorld()->GetSubsystem<USLWorldSubsystem>()->HasSaveData();
	pauseMenu->SetLoadButtonEnabled(hasSaveData);
	
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	controller->GetPawn()->DisableInput(controller);
	controller->SetShowMouseCursor(true);
	controller->SetPause(true);

	FInputModeGameAndUI mode;
	controller->SetInputMode(mode);
}

void AMyProjectGameMode::ClosePauseMenu()
{
	pauseMenu->RemoveFromParent();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	controller->GetPawn()->EnableInput(controller);
	controller->SetShowMouseCursor(false);
	controller->SetPause(false);

	FInputModeGameOnly mode;
	controller->SetInputMode(mode);
}


void AMyProjectGameMode::SpawnPlayer(const FString& IncomingName)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	RestartPlayerAtPlayerStart(controller, FindPlayerStart(controller, IncomingName));
}

void AMyProjectGameMode::RespawnPlayer(const FTransform& transform)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AActor* actor = controller->GetPawn();
	if (actor && !actor->IsActorBeingDestroyed())
	{
		GetWorld()->DestroyActor(actor);
	}
	
	RestartPlayerAtTransform(controller, transform);

}

void AMyProjectGameMode::SetGameInputAndView() const
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeGameOnly mode;
	controller->SetInputMode(mode);
	controller->SetViewTarget(controller->GetPawn());
	controller->SetShowMouseCursor(false);
}

void AMyProjectGameMode::SaveGame()
{
	GetWorld()->GetSubsystem<USLWorldSubsystem>()->SaveGame();
}

void AMyProjectGameMode::LoadGame()
{
	GetWorld()->GetSubsystem<USLWorldSubsystem>()->LoadGame();
	ClosePauseMenu();
	SetGameInputAndView();
}

void AMyProjectGameMode::RestartLevel()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->RestartLevel();
}

