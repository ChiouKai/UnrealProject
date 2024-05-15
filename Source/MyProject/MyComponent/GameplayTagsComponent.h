// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UGameplayTagsComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameplayTagsComponent();

	UPROPERTY(EditAnywhere, Category = Setting)
	FGameplayTagContainer GameplayTags;

protected:

public:	

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override
	{
		TagContainer = GameplayTags;
		return;
	}

	void AddTag(const FGameplayTag& Tag);

	void RemoveTag(const FGameplayTag& Tag);
};
