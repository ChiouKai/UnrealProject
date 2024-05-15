// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components\SphereComponent.h"
#include "InteracterInterface.h"
#include "InteracterComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FCanInteractSignature, const FString&);
DECLARE_MULTICAST_DELEGATE(FCanNotInteractSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInteracterComponent : public USphereComponent, public IInteracterInterface
{
	GENERATED_BODY()

public:	
	UInteracterComponent();

protected:

	uint8 isGrabing : 1;

	float CountTime = 0;

	TArray<class UInteractableComponent*> Interactables;

	UPROPERTY()
	UInteractableComponent* interactable;

	UPROPERTY(EditAnywhere)
	class UPhysicsHandleComponent* PhysicsHandle;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	FCanInteractSignature CanInteract;
	FCanNotInteractSignature CanNotInteract;

	void Interact();

	UFUNCTION(BlueprintCallable)
	void GrabObject_Implementation(UPrimitiveComponent* primitiveComponent) override;

	UFUNCTION(BlueprintCallable)
	void StopGrabObject_Implementation(UPrimitiveComponent* primitiveComponent) override;
};
