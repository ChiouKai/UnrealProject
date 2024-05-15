// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "W_PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UW_PauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* SaveButton;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* LoadButton;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitButton;

	void SetLoadButtonEnabled(bool value);
};
