// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSet/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UBaseAttributeSet* AS = Cast<UBaseAttributeSet>(WidgetControllerData.AttributeSet);

	if (IsValid(AS) && IsValid(AttributeInfo))
	{
		for (auto& Pair : AS->TagsToAttributes)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	}
}

void UAttributeMenuWidgetController::BroadcastUpdate()
{
	UBaseAttributeSet* AS = Cast<UBaseAttributeSet>(WidgetControllerData.AttributeSet);

	if (IsValid(AS) && IsValid(AttributeInfo))
	{
		for (auto& Pair : AS->TagsToAttributes)
		{
			WidgetControllerData.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
		}
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAttributeInfoData Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(WidgetControllerData.AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
