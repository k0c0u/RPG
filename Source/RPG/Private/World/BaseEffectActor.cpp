// Fill out your copyright notice in the Description page of Project Settings.


#include "World/BaseEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ABaseEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEffectActor::ApplyGameplayEffectToTarget(AActor* TargetActor, TSoftClassPtr<UGameplayEffect> InGameplayEffect)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectContextHandle GameplayEffectContextHandle = TargetASC->MakeEffectContext();
		GameplayEffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffect.LoadSynchronous(), 1.f, GameplayEffectContextHandle);
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}



