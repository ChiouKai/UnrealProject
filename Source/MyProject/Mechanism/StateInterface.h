// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYPROJECT_API IStateInterface
{
	GENERATED_BODY()

public:

	virtual int GetState();

	UFUNCTION(BlueprintImplementableEvent)
	void SetState(int index);
};
