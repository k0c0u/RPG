// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BaseWidgetController.h"



void UBaseWidgetController::SetWidgetControllerData(const FWidgetControllerData& Data)
{
	WidgetControllerData = Data;
}

void UBaseWidgetController::BroadcastInitialValues()
{
}

void UBaseWidgetController::BroadcastUpdate()
{
}
