// Fill out your copyright notice in the Description page of Project Settings.


#include "W_Interact.h"

void UW_Interact::ShowText(const FString& desc)
{
	Text->SetText(FText::FromString("(E) " + desc));
	
	if (!IsInViewport())
	{
		AddToViewport();
	}
}


void UW_Interact::CloseText()
{
	RemoveFromParent();
}
