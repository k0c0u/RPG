// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	None,
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSoftClassPtr<class UGameplayEffect>> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSoftClassPtr<class UGameplayAbility>> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
};



UCLASS()
class RPG_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharactersClassInformation;

	UFUNCTION(BlueprintCallable)
	FCharacterClassDefaultInfo GetCharacterClassInfo(const ECharacterClass CharacterClass);
};
