// Fill out your copyright notice in the Description page of Project Settings.


#include "SLWorldSubsystem.h"
#include "Mechanism\Device.h"
#include "Mechanism\Switcher.h"
#include "Kismet\GameplayStatics.h"
#include "MySaveGame.h"
#include "MyProjectGameMode.h"
#include "AI\AIEnemyController.h"


void USLWorldSubsystem::SaveGame()
{
	USaveGame* slot;
	if (HasSaveData())
	{
		slot = UGameplayStatics::LoadGameFromSlot(name, 0);
	}
	else
	{
		slot = UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass());
	}

	if (UMySaveGame* tmp = Cast<UMySaveGame>(slot))
	{
		tmp->ClearData();

		tmp->PlayerTransform = playerController->GetPawn()->GetActorTransform();

		for (auto enemy : enemys)
		{
			tmp->EnenmyPos.Add(enemy->NowWayPoint);
		}

		for (auto cube : cubes)
		{
			tmp->CubeTransforms.Add(cube->GetComponentTransform());
		}

		for (auto device : devices)
		{
			tmp->DeviceStates.Add(device->GetState());
		}

		for (auto switcher : switchers)
		{
			tmp->SwitcherStates.Add(switcher->GetState());
		}
	}
	UGameplayStatics::SaveGameToSlot(slot, name, 0);

}

void USLWorldSubsystem::LoadGame()
{
	if (HasSaveData())
	{
		if (UMySaveGame* slot = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(name, 0)))
		{

			GetWorld()->GetAuthGameMode<AMyProjectGameMode>()->RespawnPlayer(slot->PlayerTransform);

			for (int i = 0; i < slot->EnenmyPos.Num(); i++)
			{
				UE_LOG(LogTemp, Display, TEXT("Respawn Load"));
				enemys[i]->Respawn(slot->EnenmyPos[i]);
			}

			for (int i = 0; i < slot->CubeTransforms.Num(); i++)
			{
				cubes[i]->SetWorldTransform(slot->CubeTransforms[i], false, nullptr, ETeleportType::TeleportPhysics);
			}

			for (int i = 0; i < slot->DeviceStates.Num(); i++)
			{
				IStateInterface::Execute_SetState(devices[i], slot->DeviceStates[i]);
			}

			for (int i = 0; i < slot->SwitcherStates.Num(); i++)
			{
				IStateInterface::Execute_SetState(switchers[i], slot->SwitcherStates[i]);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("NoGameData"));
	}
}

bool USLWorldSubsystem::HasSaveData()
{
	return UGameplayStatics::DoesSaveGameExist(name, 0);
}

void USLWorldSubsystem::AddCharacter(APlayerController* player)
{
	this->playerController = player;
}

void USLWorldSubsystem::AddEnemy(AAIEnemyController* aiController)
{
	UE_LOG(LogTemp, Display, TEXT("AddEnemy"));
	enemys.AddUnique(aiController);
}

void USLWorldSubsystem::AddCubes(USceneComponent* cube)
{
	cubes.AddUnique(cube);
}

void USLWorldSubsystem::AddDevices(ADevice* device)
{
	devices.AddUnique(device);
}

void USLWorldSubsystem::AddSwitchers(ASwitcher* switcher)
{
	switchers.AddUnique(switcher);
}


