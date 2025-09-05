// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileSpellAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/CombatInterface.h"
#include "World/BaseProjectile.h"
#include "RPG/Public/GameplayTagsConteiner.h"

void UProjectileSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UProjectileSpellAbility::SpawnProjectile(const FVector& TargetLocation)
{
	const bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!IsServer)
		return;

	if (!IsValid(GetWorld()))
		return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(ProjectileClass.LoadSynchronous(), SpawnTransform,
														GetAvatarActorFromActorInfo(),
														Cast<APawn>(GetAvatarActorFromActorInfo()),
														ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);

			TArray<TWeakObjectPtr<AActor>> Actors;
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);
			FHitResult Hit;
			Hit.Location = SocketLocation;
			EffectContextHandle.AddHitResult(Hit);

			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass.LoadSynchronous(), 1.f, EffectContextHandle);
			const FGameplayTagsConteiner GameplayTags = FGameplayTagsConteiner::Get();

			for (auto& Pair : DamageTypes)
			{
				const float ScaleDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Pair.Key, ScaleDamage);
			}
			
			Projectile->DamageEffectSpecHandle = EffectSpecHandle;
		}
		
		if (IsValid(Projectile))
		{
			Projectile->SetOwner(GetAvatarActorFromActorInfo());
			Projectile->GetSphereComponent()->IgnoreActorWhenMoving(GetAvatarActorFromActorInfo(), true);
			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
