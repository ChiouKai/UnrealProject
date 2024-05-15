// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"

void UMySaveGame::ClearData()
{
	PlayerTransform = FTransform();
	EnenmyPos.Empty();
	CubeTransforms.Empty();
	DeviceStates.Empty();
	SwitcherStates.Empty();
}
