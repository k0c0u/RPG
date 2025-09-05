// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/BaseCharacter.h"
#include "Interfaces/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"


class UWidgetComponent;
class UMaterialInstance;
class ABaseAIController;
class UBehaviorTree;

UCLASS()
class RPG_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	virtual void Tick(float DeltaSeconds) override;

	virtual void PossessedBy(AController* NewController) override;

	//~ Begin Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() const override {return Level;}
	void Die() override;
	//~ End

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
protected:
	virtual void BeginPlay() override;

	void InitAbilityActorInfo();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TSoftObjectPtr<UMaterialInterface> OverlayMaterial;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Settings")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float LifeSpan =  5.f;
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings | AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<ABaseAIController> BaseAIController;

private:
	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;
};
