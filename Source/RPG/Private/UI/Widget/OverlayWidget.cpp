// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/OverlayWidget.h"


void UOverlayWidget::OnSetWidgetController_Implementation(UObject* InWidgetController)
{
	Super::OnSetWidgetController_Implementation(InWidgetController);

	HealthWidget->SetWidgetController(InWidgetController);
	ManaWidget->SetWidgetController(InWidgetController);
}
