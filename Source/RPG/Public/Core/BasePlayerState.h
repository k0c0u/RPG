// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"

#include "BasePlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
 
UCLASS()
class RPG_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePlayerState();
	
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}

	FORCEINLINE int32 GetPlayerLevel() const {return Level;}

private:
	UFUNCTION()
	void OnRep_Level(const int32& OldLevel);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
};
