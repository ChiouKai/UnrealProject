#include "GameplayTagsComponent.h"

// Sets default values for this component's properties
UGameplayTagsComponent::UGameplayTagsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameplayTagsComponent::AddTag(const FGameplayTag& Tag)
{
	GameplayTags.AddTag(Tag);
}

void UGameplayTagsComponent::RemoveTag(const FGameplayTag& Tag)
{
	GameplayTags.RemoveTag(Tag);
}




