// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsConteiner.h"
#include "InputMappingContext.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/BaseInputComponent.h"
#include "Interfaces/EnemyInterface.h"
#include "UI/Widget/DamageTextWidgetComponent.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ABasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

UBaseAbilitySystemComponent* ABasePlayerController::GetAbilitySystemComponent()
{
	if (!IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AbilitySystemComponent;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(InputMappingContext))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!IsValid(Subsystem))
	{
		return;
	}

	Subsystem->AddMappingContext(InputMappingContext, 0);

	bShowMouseCursor = true;

	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UBaseInputComponent* InputComp = CastChecked<UBaseInputComponent>(InputComponent);

	InputComp->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::Move);
	InputComp->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	InputComp->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	InputComp->BindAbilityAction(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputAxis  = Value.Get<FVector2D>();
	const FRotator Rotator = GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotator.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxis.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxis.X);
	}
}

void ABasePlayerController::ShowDamageText_Implementation(const float Damage, ACharacter* TargetCharacter,  const bool bBlockedHit, const bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && !DamageTextWidgetComponentClass.IsNull() && IsLocalController())
	{
		if (UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextWidgetComponentClass.LoadSynchronous()))
		{
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetDamageText(Damage, bBlockedHit, bCriticalHit);
		}
	}
}

void ABasePlayerController::CursorTrace()
{
	GetHitResultUnderCursor(TraceCursorChannel, true, CursorHit);
	
	if (!CursorHit.bBlockingHit)
		return;

	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();

	if (LastActor != CurrentActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (CurrentActor) CurrentActor->HighlightActor();
	}
}

void ABasePlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ABasePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FGameplayTagsConteiner::Get().InputTag_LMB))
	{
		bTargeting = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void ABasePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FGameplayTagsConteiner::Get().InputTag_LMB))
    	{
    		if (IsValid(GetAbilitySystemComponent())) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
    		return;
    	}

		if (IsValid(GetAbilitySystemComponent())) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	
    	if (!bTargeting && !bShiftKeyDown)
    	{
    		const APawn* ControlledPawn = GetPawn();
    		if (FollowTime <= ShortPressThreshold && IsValid(ControlledPawn))
    		{
    			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
    			{
    				Spline->ClearSplinePoints();
    				for (const FVector& PointLoc : NavPath->PathPoints)
    				{
    					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
    				}
    				
    				if (NavPath->PathPoints.Num() > 0)
    				{
    					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
    					bAutoRunning = true;
    				}
    			}
    		}
    		FollowTime = 0.f;
    		bTargeting = false;
    	}
}

void ABasePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FGameplayTagsConteiner::Get().InputTag_LMB))
	{
		if (IsValid(GetAbilitySystemComponent())) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (IsValid(GetAbilitySystemComponent())) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
		
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}
