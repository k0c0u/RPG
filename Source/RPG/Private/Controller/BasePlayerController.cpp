// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/BasePlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;
}

void ABasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
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

	UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Started, this, &ThisClass::Move);
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

void ABasePlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, true, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	
}
