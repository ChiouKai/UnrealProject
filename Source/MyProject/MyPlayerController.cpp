// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyProjectCharacter.h"
#include "MyComponent/GameplayTagsComponent.h"
#include "Mechanism/InteracterComponent.h"
#include "Throw/ThrowComponent.h"
#include "MyProjectGameMode.h"
#include "SLWorldSubsystem.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	GetWorld()->GetSubsystem<USLWorldSubsystem>()->AddCharacter(this);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (AMyProjectCharacter* myCharacter = Cast<AMyProjectCharacter>(aPawn))
	{
		UGameplayTagsComponent* TagsComponent = NewObject<UGameplayTagsComponent>(myCharacter);
		TagsComponent->RegisterComponent();
		TagsComponent->AddTag(FGameplayTag::RequestGameplayTag(FName("Player")));
		myCharacter->AddInstanceComponent(TagsComponent);
		UInteracterComponent* interacter = myCharacter->GetInteracter();
		interacter->CanInteract.AddUObject(this, &AMyPlayerController::AddInteractInput);
		interacter->CanNotInteract.AddUObject(this, &AMyPlayerController::RemoveInteractInput);
		myCharacter->ThrowCom->TraceChannel = ECC_Camera;

		if (AMyProjectGameMode* gameMode = Cast<AMyProjectGameMode>(GetWorld()->GetAuthGameMode()))
		{
			gameMode->SetInteractUI(interacter);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			//Jumping
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, myCharacter, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, myCharacter, &ACharacter::StopJumping);

			EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, myCharacter, &AMyProjectCharacter::Roll);
			//Moving
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, myCharacter, &AMyProjectCharacter::Move);
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::None, myCharacter, &AMyProjectCharacter::NotMove);
			//Looking
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, myCharacter, &AMyProjectCharacter::Look);

			EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, myCharacter, &AMyProjectCharacter::Throw);

			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, myCharacter, &AMyProjectCharacter::Aim);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, myCharacter, &AMyProjectCharacter::AimTrace);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, myCharacter, &AMyProjectCharacter::Aim);

			EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AMyPlayerController::PauseGame);
		}
	}
}

void AMyPlayerController::OnUnPossess()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionBindings();
	}

	if (AMyProjectCharacter* myCharacter = Cast<AMyProjectCharacter>(GetPawn()))
	{
		UGameplayTagsComponent* tags = myCharacter->GetComponentByClass<UGameplayTagsComponent>();
		if (tags)
		{
			myCharacter->RemoveInstanceComponent(tags);
		}

		UInteracterComponent* interacter = myCharacter->GetInteracter();
		interacter->CanInteract.Clear();
		interacter->CanNotInteract.Clear();

		myCharacter->ThrowCom->TraceChannel = ECC_Visibility;
	}
}

void AMyPlayerController::AddInteractInput(const FString& desc)
{
	if (InteractHandle == -1)
	{
		if (AMyProjectCharacter* myCharacter = Cast<AMyProjectCharacter>(GetPawn()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
			{
				FEnhancedInputActionEventBinding& bindingHandle = EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, myCharacter, &AMyProjectCharacter::InteractEnvironment);
				InteractHandle = bindingHandle.GetHandle();
			}
		}
	}
}

void AMyPlayerController::RemoveInteractInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->RemoveBindingByHandle(InteractHandle);
		InteractHandle = -1;
	}
}

void AMyPlayerController::PauseGame()
{
	if (!IsPaused())
	{
		GetWorld()->GetAuthGameMode<AMyProjectGameMode>()->ShowPauseMenu();
	}
	else
	{
		GetWorld()->GetAuthGameMode<AMyProjectGameMode>()->ClosePauseMenu();
	}

}

