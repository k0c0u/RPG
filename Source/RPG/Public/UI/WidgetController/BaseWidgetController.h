// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseWidgetController.generated.h"

class APlayerController;
class APlayerState;
class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerData
{
	GENERATED_BODY()

	FWidgetControllerData() {};

	FWidgetControllerData(APlayerController* PC, APlayerState* APS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(APS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class RPG_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerData(const FWidgetControllerData& Data);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BroadcastUpdate();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	FWidgetControllerData WidgetControllerData;
};
