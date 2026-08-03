// ThunderFighter - 雷霆战机 GameMode 实现

#include "ThunderFighterGameMode.h"
#include "ThunderFighterPlayerController.h"
#include "ThunderFighterGameInstance.h"
#include "UI/ThunderFighterHUD.h"
#include "Actors/EnemySpawner.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AThunderFighterGameMode::AThunderFighterGameMode()
{
	// 允许 Tick
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.0f;
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

	// 更新历史最高分
	int32 HighScore = CurrentScore;
	if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
	{
		GI->TryUpdateHighScore(CurrentScore);
		HighScore = GI->GetHighScore();
	}

	// 显示游戏结束结算界面
	AThunderFighterPlayerController* PC = Cast<AThunderFighterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		if (AThunderFighterHUD* HUD = Cast<AThunderFighterHUD>(PC->GetHUD()))
		{
			HUD->ShowGameOverScreen(CurrentScore, HighScore);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player defeated! Final Score: %d | High Score: %d"), CurrentScore, HighScore);
}

void AThunderFighterGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AThunderFighterGameMode::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
