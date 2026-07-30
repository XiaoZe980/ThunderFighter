// ThunderFighter - 雷霆战机 PlayerController Implementation

#include "ThunderFighterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Actors/ThunderFighterPlayerPawn.h"

AThunderFighterPlayerController::AThunderFighterPlayerController()
{
	// Allow tick for input processing
	PrimaryActorTick.bCanEverTick = true;
}

void AThunderFighterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Diagnostic: check if input assets are configured
	if (!GameplayInputMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("[ThunderFighter] PlayerController: GameplayInputMapping is NULL! Set it in the BP class defaults."));
	}
	if (!IA_MoveHorizontal)
	{
		UE_LOG(LogTemp, Error, TEXT("[ThunderFighter] PlayerController: IA_MoveHorizontal is NULL! Assign it in the BP."));
	}

	// Add the gameplay input mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (GameplayInputMapping)
		{
			Subsystem->AddMappingContext(GameplayInputMapping, 0);
			UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Input Mapping Context added successfully."));
		}
	}
}

void AThunderFighterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput) return;

	// Bind movement actions
	if (IA_MoveHorizontal)
	{
		EnhancedInput->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this,
			&AThunderFighterPlayerController::OnMoveHorizontal);
		EnhancedInput->BindAction(IA_MoveHorizontal, ETriggerEvent::Completed, this,
			&AThunderFighterPlayerController::OnMoveHorizontal);
	}

	if (IA_MoveVertical)
	{
		EnhancedInput->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this,
			&AThunderFighterPlayerController::OnMoveVertical);
		EnhancedInput->BindAction(IA_MoveVertical, ETriggerEvent::Completed, this,
			&AThunderFighterPlayerController::OnMoveVertical);
	}

	// Bind fire action
	if (IA_Fire)
	{
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnFireStarted);
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Completed, this,
			&AThunderFighterPlayerController::OnFireStopped);
	}

	// Bind bomb action
	if (IA_Bomb)
	{
		EnhancedInput->BindAction(IA_Bomb, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnBomb);
	}

	// Bind pause action
	if (IA_Pause)
	{
		EnhancedInput->BindAction(IA_Pause, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnPause);
	}
}

// -- Input Callbacks --

void AThunderFighterPlayerController::OnMoveHorizontal(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->AddMovementInput(FVector(0.0f, Value.Get<float>(), 0.0f));
	}
}

void AThunderFighterPlayerController::OnMoveVertical(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->AddMovementInput(FVector(Value.Get<float>(), 0.0f, 0.0f));
	}
}

void AThunderFighterPlayerController::OnFireStarted(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->StartFiring();
	}
}

void AThunderFighterPlayerController::OnFireStopped(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->StopFiring();
	}
}

void AThunderFighterPlayerController::OnBomb(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		PlayerPawn->UseBomb();
	}
}

void AThunderFighterPlayerController::OnPause(const FInputActionValue& Value)
{
	// Pause logic — to be expanded with UI
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Pause requested"));
}
