// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/AttributeSet/BaseAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "RPG/RPG.h"
#include "GameplayTagsConteiner.h"
#include "GameFramework/CharacterMovementComponent.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Prokectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
} 

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTagsConteiner::Get().EffectsHitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ThisClass::HitReactTagChanged);
	}
}

void ABaseCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void ABaseCharacter::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

void ABaseCharacter::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
}

void ABaseCharacter::SetCharacterData(UCharacterClassInfo* Data)
{
	if (IsValid(Data))
	{
		CharacterData = Data;
	}
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterClassInfo))
	{
		SetCharacterData(CharacterClassInfo);
	}
}


FVector ABaseCharacter::GetCombatSocketLocation() const
{
	return Weapon->GetSocketLocation(WeaponSocket);
}


void ABaseCharacter::GiveAbilities()
{
	if (HasAuthority() && IsValid(GetAbilitySystemComponent()))
	{
		for (auto& DefaultAbility : CharacterData->GetCharacterClassInfo(CharacterClass).Abilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(DefaultAbility.LoadSynchronous(), 1);
			if(const UBaseGameplayAbility* Ability = Cast<UBaseGameplayAbility>(AbilitySpec.Ability))
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->InputTag);
				GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
			}
			else
			{
				GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
			}
		}
	}
}

void ABaseCharacter::ApplyStartupEffects()
{
	if (GetLocalRole() == ROLE_Authority && IsValid(GetAbilitySystemComponent()))
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		for (auto& Effect : CharacterData->GetCharacterClassInfo(CharacterClass).Effects)
		{
			ApplyGameplayEffectToSelf(Effect, EffectContext);
		}
	}
}

bool ABaseCharacter::ApplyGameplayEffectToSelf(TSoftClassPtr<UGameplayEffect>& Effect, const FGameplayEffectContextHandle& EffectContext)
{
	if (Effect.IsNull() || !IsValid(GetAbilitySystemComponent()))
		return false;
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect.LoadSynchronous(), 1, EffectContext);
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle  ActiveGameplayEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	return false;
}

void ABaseCharacter::OnRep_CharacterData()
{
	
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

void ABaseCharacter::Dissolve()
{
	if (IsValid(DissolveMaterial))
	{
		if (UMaterialInstanceDynamic* DynamicMat = UMaterialInstanceDynamic::Create(DissolveMaterial, this))
		{
			GetMesh()->SetMaterial(0, DynamicMat);
			StartDissolveTimeline(DynamicMat);
		}
	}

	if (IsValid(WeaponDissolveMaterial))
	{
		if (UMaterialInstanceDynamic* WeaponDynamicMat = UMaterialInstanceDynamic::Create(WeaponDissolveMaterial, this))
		{
			Weapon->SetMaterial(0, WeaponDynamicMat);
			StartWeaponDissolveTimeline(WeaponDynamicMat);
		}
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, CharacterData);
}
