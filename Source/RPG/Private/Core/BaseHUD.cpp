// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/BaseUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
}

UOverlayWidgetController* ABaseHUD::GetOverlayWidgetController(const FWidgetControllerData& Data)
{
	if (!IsValid(OverlayWidgetController))
	{
		if (!OverlayWidgetControllerClass.IsNull())
		{
			UClass* ControllerClass = OverlayWidgetControllerClass.LoadSynchronous();
			if (IsValid(ControllerClass))
			{
				OverlayWidgetController = NewObject<UOverlayWidgetController>(this, ControllerClass);
				if (IsValid(OverlayWidgetController))
				{
					OverlayWidgetController->SetWidgetControllerData(Data);
					OverlayWidgetController->BroadcastUpdate();
				}
			}
		}
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ABaseHUD::GetAttributeMenuWidgetController(const FWidgetControllerData& Data)
{
	if (!IsValid(AttributeMenuWidgetController))
	{
		if (!AttributeMenuWidgetControllerClass.IsNull())
		{
			UClass* ControllerClass = AttributeMenuWidgetControllerClass.LoadSynchronous();
			if (IsValid(ControllerClass))
			{
				AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, ControllerClass);
				if (IsValid(AttributeMenuWidgetController))
				{
					AttributeMenuWidgetController->SetWidgetControllerData(Data);
					AttributeMenuWidgetController->BroadcastUpdate();
				}
			}
		}
	}
	return AttributeMenuWidgetController;
}

void ABaseHUD::Init(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (!OverlayWidgetClass.IsNull())
	{
		OverlayWidget = CreateWidget<UBaseUserWidget>(GetWorld(), OverlayWidgetClass.LoadSynchronous());
		if (IsValid(OverlayWidget))
		{
			const FWidgetControllerData Data (PC, PS, ASC, AS);
			if (UOverlayWidgetController* WidgetController = GetOverlayWidgetController(Data))
			{
				OverlayWidget->SetWidgetController(WidgetController);
				WidgetController->BroadcastInitialValues();
			}
			
			OverlayWidget->AddToViewport();
		}
	}
}
