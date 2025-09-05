// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class RPG_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ABaseEffectActor();
	virtual void Tick(float DeltaTime) override;
	
protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* TargetActor, TSoftClassPtr<UGameplayEffect> InGameplayEffect);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	TSoftClassPtr<UGameplayEffect> GameplayEffect;
};
