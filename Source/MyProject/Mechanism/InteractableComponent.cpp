// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"
#include "InteracterInterface.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponent::InteractTest(TScriptInterface<IInteracterInterface> IInteract)
{
	TSIInteract = IInteract;
	if (InteractDelegate.IsBound())
	{
		InteractDelegate.Broadcast(TSIInteract);
	}
}

