// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	TSoftClassPtr<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
