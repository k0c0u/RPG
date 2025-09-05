// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/BaseUserWidget.h"


void UBaseUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBaseUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (IsValid(InWidgetController))
	{
		WidgetController = InWidgetController;
		OnSetWidgetController(WidgetController);
	}
}

void UBaseUserWidget::OnSetWidgetController_Implementation(UObject* InWidgetController)
{
}
