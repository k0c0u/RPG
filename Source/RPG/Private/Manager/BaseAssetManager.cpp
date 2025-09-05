// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/BaseAssetManager.h"

#include "GameplayTagsConteiner.h"
#include "AbilitySystemGlobals.h"


UBaseAssetManager& UBaseAssetManager::Get()
{
	return *Cast<UBaseAssetManager>(GEngine->AssetManager);
}

void UBaseAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FGameplayTagsConteiner::InitializeGameplayTags();

	// use target data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
