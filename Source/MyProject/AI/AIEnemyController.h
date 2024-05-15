// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()


	AAIEnemyController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStateTreeComponent* StateTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerception;
	
protected:
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bDetectPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AWayPoint* NowWayPoint;

	UFUNCTION(BlueprintCallable)
	class AWayPoint* GetRandomPoint();

	void Respawn(class AWayPoint* wayPoint);
};
