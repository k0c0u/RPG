// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class RPG_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	
	void BroadcastInitialValues() override;
	void BroadcastUpdate() override;

	void OnHealthAttributChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthAttributChanged(const FOnAttributeChangeData& Data) const;

	void OnManaAttributChanged(const FOnAttributeChangeData& Data) const ;
	void OnMaxManaAttributChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnMaxManaChanged;
};
