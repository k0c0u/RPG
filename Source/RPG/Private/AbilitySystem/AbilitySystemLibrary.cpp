// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemLibrary.h"

#include "AbilityTypes.h"
#include "Core/BaseGameMode.h"
#include "Core/BaseHUD.h"
#include "Core/BasePlayerState.h"
#include "UI/WidgetController/BaseWidgetController.h"



UOverlayWidgetController* UAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!IsValid(World) && World->GetFirstPlayerController())
	{
		return nullptr;
	}
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	ABaseHUD* HUD = Cast<ABaseHUD>(PlayerController->GetHUD());
	if (!IsValid(HUD))
	{
		return nullptr;
	}

	ABasePlayerState* PS = PlayerController->GetPlayerState<ABasePlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet* AS = PS->GetAttributeSet();
	const FWidgetControllerData WidgetControllerData(PlayerController, PS, ASC, AS);
	
	return HUD->GetOverlayWidgetController(WidgetControllerData);
}

UAttributeMenuWidgetController* UAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (!IsValid(World) && World->GetFirstPlayerController())
	{
		return nullptr;
	}
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	ABaseHUD* HUD = Cast<ABaseHUD>(PlayerController->GetHUD());
	if (!IsValid(HUD))
	{
		return nullptr;
	}

	ABasePlayerState* PS = PlayerController->GetPlayerState<ABasePlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet* AS = PS->GetAttributeSet();
	const FWidgetControllerData WidgetControllerData(PlayerController, PS, ASC, AS);
	
	return HUD->GetAttributeMenuWidgetController(WidgetControllerData);
}

bool UAbilitySystemLibrary::IsBlockedDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseContext->IsBlockedHit();
	}
	return false;
}

bool UAbilitySystemLibrary::IsCriticalkDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FBaseGameplayEffectContext* BaseContext = static_cast<const FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return BaseContext->IsCriticalHit();
	}
	return false;
}

void UAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bBlocked)
{
	if (FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseContext->SetBlockedHit(bBlocked);
	}
}

void UAbilitySystemLibrary::SetCriticalkHit(FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalkHit)
{
	if (FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		BaseContext->SetCriticalHit(bCriticalkHit);
	}
}
