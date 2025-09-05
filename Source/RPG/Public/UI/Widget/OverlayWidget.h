// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/BaseUserWidget.h"
#include "OverlayWidget.generated.h"



 
UCLASS()
class RPG_API UOverlayWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void OnSetWidgetController_Implementation(UObject* WidgetController) override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseUserWidget> HealthWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseUserWidget> ManaWidget;
	
};
