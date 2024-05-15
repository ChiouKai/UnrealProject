// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteracterInterface.h"
#include "InteractableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractSignature, TScriptInterface<IInteracterInterface>, IInteract);

UENUM()
enum class EInteractMode : uint8
{
	EI_Once,
	EI_Continuous
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInteractMode Mode;

	UPROPERTY(BlueprintAssignable)
	FInteractSignature InteractDelegate;

	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteracterInterface> TSIInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION()
	void InteractTest(TScriptInterface<IInteracterInterface> IInteract);
};