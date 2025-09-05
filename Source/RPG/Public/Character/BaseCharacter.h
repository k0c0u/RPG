// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemReplicationProxyInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interfaces/CombatInterface.h"

#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAnimMontage;
class UMaterialInstanceDynamic;

UCLASS(Abstract)
class RPG_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
	void SetCharacterData(UCharacterClassInfo* Data);

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	TObjectPtr<UCurveTable> GetDamageCalculationCoefficients() const {return CharacterData->GetCharacterClassInfo(CharacterClass).DamageCalculationCoefficients;}

protected:
	
	virtual void BeginPlay() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}

	virtual FVector GetCombatSocketLocation() const override;
	
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	void GiveAbilities();
	void ApplyStartupEffects();

	bool ApplyGameplayEffectToSelf(TSoftClassPtr<UGameplayEffect>& Effect, const FGameplayEffectContextHandle& EffectContext);

	UFUNCTION()
	void OnRep_CharacterData();

	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

private:
	void Dissolve();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Settings|Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|Combat")
	float  BaseWalkSpeed = 600.f;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (ClampMin = "0"))
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Weapon")
	FName WeaponSocket = TEXT("WeaponHandSocket");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Weapon")
	FName WeaponTipSocketName = TEXT("TipSocket");

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Settings|BaseCharacter")
	class UCharacterClassInfo* CharacterClassInfo;

	UPROPERTY(ReplicatedUsing=OnRep_CharacterData)
	TObjectPtr<UCharacterClassInfo> CharacterData;

	UPROPERTY(EditAnywhere, Category = "Settings|Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UMaterialInstance> DissolveMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterial;
};
