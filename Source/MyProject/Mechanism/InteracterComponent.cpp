// Fill out your copyright notice in the Description page of Project Settings.


#include "InteracterComponent.h"
#include "InteractableComponent.h"
#include "PhysicsEngine\PhysicsHandleComponent.h"
#include "Kismet\KismetMathLibrary.h"


UInteracterComponent::UInteracterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	OnComponentBeginOverlap.AddDynamic(this, &UInteracterComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteracterComponent::OnEndOverlap);
}


// Called when the game starts
void UInteracterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteracterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (isGrabing)
	{
		FVector grabbedLocation = PhysicsHandle->GetGrabbedComponent()->GetComponentLocation();
		float distance = FVector::Distance(grabbedLocation, GetComponentLocation());
		if (distance > 0.1)
		{
			FVector target = FMath::VInterpTo(grabbedLocation, GetComponentLocation(), DeltaTime, 50.0f);
			PhysicsHandle->SetTargetLocation(target);
		}
	}

	if (Interactables.Num() > 0)
	{
		CountTime += DeltaTime;
		if (CountTime > 0.2f)
		{
			CountTime = 0.0f;

			double tmpDistance = DBL_MAX;
			UInteractableComponent* tmpInteractable = nullptr;
			for (auto tmp : Interactables)
			{
				double dis = (GetComponentLocation() - tmp->GetOwner()->GetActorLocation()).Length();
				if (tmpDistance > dis)
				{
					tmpDistance = dis;
					tmpInteractable = tmp;
				}
			}

			if (interactable != tmpInteractable)
			{
				interactable = tmpInteractable;
				CanInteract.Broadcast(interactable->Description);
			}
		}
	}
}

void UInteracterComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UInteractableComponent* tmp = OtherActor->GetComponentByClass<UInteractableComponent>())
	{
		Interactables.AddUnique(tmp);
	}
}

void UInteracterComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(UInteractableComponent* tmp = OtherActor->GetComponentByClass<UInteractableComponent>())
	{
		Interactables.Remove(tmp);
		if (Interactables.Num() == 0)
		{
			interactable = nullptr;
			CanNotInteract.Broadcast();
		}
	}

}

void UInteracterComponent::Interact()
{
	interactable->InteractTest(this);//might trigger OnEndOverlap

	if (interactable)
	{
		CanInteract.Broadcast(interactable->Description);
	}
}

void UInteracterComponent::GrabObject_Implementation(UPrimitiveComponent* primitiveComponent)
{
	if (!isGrabing)
	{
		isGrabing = 1;
		PhysicsHandle->GrabComponentAtLocation(primitiveComponent, NAME_None, primitiveComponent->GetComponentLocation());
	}
}

void UInteracterComponent::StopGrabObject_Implementation(UPrimitiveComponent* primitiveComponent)
{
	if (isGrabing && PhysicsHandle->GetGrabbedComponent() == primitiveComponent)
	{
		isGrabing = 0;
		PhysicsHandle->ReleaseComponent();
	}
}
