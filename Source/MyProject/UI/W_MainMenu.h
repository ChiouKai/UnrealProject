// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "W_MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API UW_MainMenu : public UUserWidget
{
	GENERATED_BODY()


public:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitButton;

	void SetContinueButtonEnabled(bool value);
};
