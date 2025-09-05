// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AttributeSet/BaseAttributeSet.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	if (IsValid(WidgetControllerData.AttributeSet))
	{
		if (const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(WidgetControllerData.AttributeSet))
		{
			OnHealthChanged.Broadcast(BaseAttributeSet->GetHealth());
			OnMaxHealthChanged.Broadcast(BaseAttributeSet->GetMaxHealth());
			OnManaChanged.Broadcast(BaseAttributeSet->GetMana());
			OnMaxManaChanged.Broadcast(BaseAttributeSet->GetMaxMana());
		}
	}
}

void UOverlayWidgetController::BroadcastUpdate()
{
	if (const UBaseAttributeSet* BaseAttributeSet = CastChecked<UBaseAttributeSet>(WidgetControllerData.AttributeSet))
	{
		if (IsValid(WidgetControllerData.AbilitySystemComponent))
		{
			WidgetControllerData.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthAttributChanged);
			WidgetControllerData.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthAttributChanged);
			WidgetControllerData.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetManaAttribute()).AddUObject(this, &ThisClass::OnManaAttributChanged);
			WidgetControllerData.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(BaseAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnMaxManaAttributChanged);
		}
	}
}

void UOverlayWidgetController::OnHealthAttributChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthAttributChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnManaAttributChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxManaAttributChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
