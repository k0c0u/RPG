

#include "Character/EnemyCharacter.h"

#include "GameplayTagsConteiner.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/BaseAttributeSet.h"
#include "AI/BaseAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/BaseUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystem_Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitAbilityActorInfo();

	if (UBaseUserWidget* UserWidget = Cast<UBaseUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UserWidget->SetWidgetController(this);
	}

	if (IsValid(AbilitySystemComponent))
	{
		if (const UBaseAttributeSet* AS = Cast<UBaseAttributeSet>(AttributeSet))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
				{
					OnHealthChanged.Broadcast(Data.NewValue);
				}
			);

			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
				{
					OnMaxHealthChanged.Broadcast(Data.NewValue);
				}
			);

			AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTagsConteiner::Get().EffectsHitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ThisClass::HitReactTagChanged
		);

			OnHealthChanged.Broadcast(AS->GetHealth());
			OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
		}
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

		if (HasAuthority())
		{
			GiveAbilities();
			ApplyStartupEffects();
		}
	}
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (IsValid(BaseAIController) && IsValid(BaseAIController->GetBlackboardComponent()))
	{
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (!HasAuthority())
		return;
	
	BaseAIController = Cast<ABaseAIController>(NewController);
	if (IsValid(BaseAIController) && IsValid(BehaviorTree))
	{
		BaseAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BaseAIController->RunBehaviorTree(BehaviorTree);
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
		BaseAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	}
}

void AEnemyCharacter::HighlightActor()
{
	GetMesh()->SetOverlayMaterial(OverlayMaterial.LoadSynchronous());
}


void AEnemyCharacter::UnHighlightActor()
{
	GetMesh()->SetOverlayMaterial(nullptr);
}