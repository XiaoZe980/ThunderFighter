// ThunderFighter - 雷霆战机 GameMode 实现

#include "ThunderFighterGameMode.h"
#include "ThunderFighterPlayerController.h"
#include "ThunderFighterGameInstance.h"
#include "UpgradeSystem.h"
#include "UI/ThunderFighterHUD.h"
#include "UI/UpgradeSelectWidget.h"
#include "Actors/EnemySpawner.h"
#include "Actors/ThunderFighterPlayerPawn.h"
#include "Components/PlayerLevelComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AThunderFighterGameMode::AThunderFighterGameMode()
{
	// 允许 Tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.0f;

	// 强化系统组件
	UpgradeSystem = CreateDefaultSubobject<UUpgradeSystem>(TEXT("UpgradeSystem"));
}

void AThunderFighterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 在关卡中查找敌人生成器
	for (TActorIterator<AEnemySpawner> It(GetWorld()); It; ++It)
	{
		EnemySpawnerRef = *It;
		break;
	}

	// 绑定玩家升级事件
	AThunderFighterPlayerPawn* Player = Cast<AThunderFighterPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
	{
		if (UPlayerLevelComponent* Level = Player->FindComponentByClass<UPlayerLevelComponent>())
		{
			Level->OnLevelUp.AddDynamic(this, &AThunderFighterGameMode::OnPlayerLevelUp);
		}
	}
}

void AThunderFighterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bGameOver)
	{
		GameOverTimer -= DeltaTime;
		if (GameOverTimer <= 0.0f)
		{
			// 触发游戏结束 UI 或关卡转换
			// 默认：延迟后重新开始（可在蓝图中重写）
		}
	}
}

void AThunderFighterGameMode::AddScore(int32 Points)
{
	if (bGameOver) return;

	CurrentScore += Points;

	// 广播分数变更事件（可由 HUD 绑定）
	// 使用简单委托——后续可扩展
}

void AThunderFighterGameMode::OnPlayerDefeated()
{
	if (bGameOver) return;

	bGameOver = true;
	GameOverTimer = GameOverDelay;

	// 停止敌人生成
	if (EnemySpawnerRef.IsValid())
	{
		EnemySpawnerRef->StopSpawning();
	}

	// 更新历史最高分 + 结算金币
	int32 HighScore = CurrentScore;
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->TryUpdateHighScore(CurrentScore);
		HighScore = GI->GetHighScore();

		// 按分数结算金币（每 100 分 1 金币），并保存
		int32 EarnedGold = CurrentScore / 100;
		if (EarnedGold > 0)
		{
			GI->AddGold(EarnedGold);
			GI->SaveData();
			UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 本局获得 %d 金币"), EarnedGold);
		}
	}

	// 显示游戏结束结算界面
	AThunderFighterPlayerController* PC = Cast<AThunderFighterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		if (AThunderFighterHUD* HUD = Cast<AThunderFighterHUD>(PC->GetHUD()))
		{
			HUD->ShowGameOverScreen(CurrentScore, HighScore);
		}

		// 切换为鼠标 + UI 输入模式，让玩家能点击结算界面按钮
		PC->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player defeated! Final Score: %d | High Score: %d"), CurrentScore, HighScore);
}

void AThunderFighterGameMode::OnPlayerLevelUp()
{
	// 防止重复触发（比如连续升级正在处理中）
	if (bIsChoosingUpgrade) return;

	bIsChoosingUpgrade = true;

	// 抽取 3 个强化候选
	if (UpgradeSystem)
	{
		PendingUpgradeOptions = UpgradeSystem->DrawOptions();
	}

	// 暂停游戏，防止选择期间继续战斗
	UGameplayStatics::SetGamePaused(this, true);

	// 通过 PlayerController 的 HUD 弹出三选一界面
	AThunderFighterPlayerController* PC = Cast<AThunderFighterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		// 切换为鼠标 + UI 输入模式，让玩家能点击强化卡
		PC->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);

		if (AThunderFighterHUD* HUD = Cast<AThunderFighterHUD>(PC->GetHUD()))
		{
			HUD->ShowUpgradeSelect(PendingUpgradeOptions, this);
		}
		else
		{
			// 没有 HUD 时直接应用第一个（容错）
			OnUpgradeChosen(0);
		}
	}
	else
	{
		OnUpgradeChosen(0);
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 升级! 弹出 %d 个强化选项"), PendingUpgradeOptions.Num());
}

void AThunderFighterGameMode::OnUpgradeChosen(int32 SelectedIndex)
{
	// 应用选择的强化
	if (UpgradeSystem && PendingUpgradeOptions.IsValidIndex(SelectedIndex))
	{
		UpgradeSystem->ApplyUpgrade(PendingUpgradeOptions[SelectedIndex]);
	}

	// 关闭强化界面
	AThunderFighterPlayerController* PC = Cast<AThunderFighterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		if (AThunderFighterHUD* HUD = Cast<AThunderFighterHUD>(PC->GetHUD()))
		{
			HUD->HideUpgradeSelect();
		}

		// 恢复游戏输入模式，隐藏鼠标
		PC->bShowMouseCursor = false;
		FInputModeGameOnly GameMode;
		PC->SetInputMode(GameMode);
	}

	// 恢复游戏
	bIsChoosingUpgrade = false;
	UGameplayStatics::SetGamePaused(this, false);
}

void AThunderFighterGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AThunderFighterGameMode::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
