// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "W_Interact.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UW_Interact : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite, meta = (BindWidget))
	UBorder* Background;

	UPROPERTY(VisibleDefaultsOnly , BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;
	
	UFUNCTION()
	void ShowText(const FString& desc);

	UFUNCTION()
	void CloseText();
};
