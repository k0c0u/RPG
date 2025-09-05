// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "ProjectileSpellAbility.generated.h"


class ABaseProjectile;
class UGameplayEffect;
 
UCLASS()
class RPG_API UProjectileSpellAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = Projectile)
	void SpawnProjectile(const FVector& TargetLocation);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<ABaseProjectile> ProjectileClass;
};
