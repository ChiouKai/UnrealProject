// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "WayPoint.h"
#include "Components/StateTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "MyProject/MyComponent/GameplayTagsComponent.h"
#include "MyProject/SLWorldSubsystem.h"
#include "Kismet\KismetSystemLibrary.h"




AAIEnemyController::AAIEnemyController()
{
	StateTree = CreateDefaultSubobject<UStateTreeComponent>(FName("StateTree"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerception"));
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnPerceptionUpdated);
}


void AAIEnemyController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<USLWorldSubsystem>()->AddEnemy(this);

	if (!IsValid(NowWayPoint))
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

		UClass* seekClass = AWayPoint::StaticClass();
		TArray<AActor*> ignoreActors;

		TArray<AActor*> outResult;

		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetPawn()->GetActorLocation(), 200.0f, objectTypes, seekClass, ignoreActors, outResult);
		
		if (outResult.Num() > 0)
		{
			NowWayPoint = reinterpret_cast<AWayPoint*> (outResult[0]);
		}
	}

	StateTree->StartLogic();
}

void AAIEnemyController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UGameplayTagsComponent* gameplayTags = Actor->GetComponentByClass<UGameplayTagsComponent>())
	{
		if (gameplayTags->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Player")))
		{
			bDetectPlayer = Stimulus.WasSuccessfullySensed();//location//vec
			Player = Actor;
		}
	}
}

AWayPoint* AAIEnemyController::GetRandomPoint()
{
	return NowWayPoint->GetRandomPoint();
}

void AAIEnemyController::Respawn(AWayPoint* wayPoint)
{
	UE_LOG(LogTemp, Display, TEXT("Respawn"));
	NowWayPoint = wayPoint;
	bDetectPlayer = false;
	GetPawn()->SetActorLocation(NowWayPoint->GetActorLocation());
	GetPawn()->Reset();
	StateTree->RestartLogic();
}

