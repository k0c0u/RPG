// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
class UBaseUserWidget;
class UOverlayWidgetController;
struct FWidgetControllerData;
class UAttributeMenuWidgetController;

UCLASS()
class RPG_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerData& Data);

	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerData& Data);

	void Init(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY()
	TObjectPtr<UBaseUserWidget> OverlayWidget = nullptr;;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UBaseUserWidget> OverlayWidgetClass = nullptr;;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UOverlayWidgetController> OverlayWidgetControllerClass = nullptr;;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController = nullptr;
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass = nullptr;
};
