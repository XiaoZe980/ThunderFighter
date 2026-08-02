// ThunderFighter - 雷霆战机 PlayerController 实现

#include "ThunderFighterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Actors/ThunderFighterPlayerPawn.h"

AThunderFighterPlayerController::AThunderFighterPlayerController()
{
	// 允许 Tick 以处理输入
	PrimaryActorTick.bCanEverTick = true;
}

void AThunderFighterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 诊断：检查输入资产是否已配置
	if (!GameplayInputMapping)
	{
		UE_LOG(LogTemp, Error, TEXT("[ThunderFighter] PlayerController: GameplayInputMapping is NULL! Set it in the BP class defaults."));
	}
	if (!IA_MoveHorizontal)
	{
		UE_LOG(LogTemp, Error, TEXT("[ThunderFighter] PlayerController: IA_MoveHorizontal is NULL! Assign it in the BP."));
	}

	// 添加游戏输入映射上下文
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

	// 绑定移动动作
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

	// 绑定开火动作
	if (IA_Fire)
	{
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnFireStarted);
		EnhancedInput->BindAction(IA_Fire, ETriggerEvent::Completed, this,
			&AThunderFighterPlayerController::OnFireStopped);
	}

	// 绑定炸弹动作
	if (IA_Bomb)
	{
		EnhancedInput->BindAction(IA_Bomb, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnBomb);
	}

	// 绑定暂停动作
	if (IA_Pause)
	{
		EnhancedInput->BindAction(IA_Pause, ETriggerEvent::Started, this,
			&AThunderFighterPlayerController::OnPause);
	}
}

// -- 输入回调 --

void AThunderFighterPlayerController::OnMoveHorizontal(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		// 横屏布局：左右键 = 前后移动（世界 X），相机跟随
		PlayerPawn->AddMovementInput(FVector(Value.Get<float>(), 0.0f, 0.0f));
	}
}

void AThunderFighterPlayerController::OnMoveVertical(const FInputActionValue& Value)
{
	AThunderFighterPlayerPawn* PlayerPawn = Cast<AThunderFighterPlayerPawn>(GetPawn());
	if (PlayerPawn)
	{
		// 横屏布局：上下键 = 横向移动（世界 Y），相机不跟随
		PlayerPawn->AddMovementInput(FVector(0.0f, Value.Get<float>(), 0.0f));
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
	// 暂停逻辑——后续将配合 UI 扩展
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Pause requested"));
}
