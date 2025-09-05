// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/AssetManager.h"
#include "BaseAssetManager.generated.h"


UCLASS()
class RPG_API UBaseAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UBaseAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
