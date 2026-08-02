// ThunderFighter - 雷霆战机 HUD 实现

#include "ThunderFighterHUD.h"
#include "BossHealthBarWidget.h"
#include "Actors/BossEnemy.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"

AThunderFighterHUD::AThunderFighterHUD()
{
	// 需要每帧轮询场景中的 Boss 以更新/隐藏血条
	PrimaryActorTick.bCanEverTick = true;
}

void AThunderFighterHUD::BeginPlay()
{
	Super::BeginPlay();

	// 游戏开始时显示游戏玩法 HUD
	ShowGameplayHUD();
}

void AThunderFighterHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 查找场景中是否存在 Boss
	ABossEnemy* Boss = nullptr;
	for (TActorIterator<ABossEnemy> It(GetWorld()); It; ++It)
	{
		Boss = *It;
		break;
	}

	if (Boss)
	{
		// 有 Boss：确保血条显示并更新
		ShowBossHealthBar();

		if (BossHealthBarWidget)
		{
			UBossHealthBarWidget* BarWidget = Cast<UBossHealthBarWidget>(BossHealthBarWidget);
			if (BarWidget)
			{
				BarWidget->SetBossInfo(Boss->GetHealthPercent(), Boss->GetActorLabel());
			}
		}
	}
	else
	{
		// 无 Boss（含被击败后销毁）：隐藏血条
		HideBossHealthBar();
	}
}

void AThunderFighterHUD::ShowGameplayHUD()
{
	if (GameplayHUDWidget || !GameplayHUDClass) return;

	GameplayHUDWidget = CreateWidget<UUserWidget>(GetWorld(), GameplayHUDClass);
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->AddToViewport();
	}
}

void AThunderFighterHUD::HideGameplayHUD()
{
	if (GameplayHUDWidget)
	{
		GameplayHUDWidget->RemoveFromParent();
		GameplayHUDWidget = nullptr;
	}
}

void AThunderFighterHUD::ShowGameOverScreen(int32 FinalScore, int32 HighScore)
{
	HideGameplayHUD();

	if (!GameOverWidgetClass) return;

	GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->AddToViewport(10); // 高 Z-Order
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Game Over! Score: %d | High Score: %d"), FinalScore, HighScore);
}

void AThunderFighterHUD::ShowPauseMenu()
{
	if (PauseMenuWidget || !PauseMenuWidgetClass) return;

	PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseMenuWidget)
	{
		PauseMenuWidget->AddToViewport(10);
	}
}

void AThunderFighterHUD::HidePauseMenu()
{
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}
}

void AThunderFighterHUD::ShowBossHealthBar()
{
	// 已显示则跳过（幂等）
	if (BossHealthBarWidget) return;
	if (!BossHealthBarClass) return;

	BossHealthBarWidget = CreateWidget<UUserWidget>(GetWorld(), BossHealthBarClass);
	if (BossHealthBarWidget)
	{
		// 高 Z-Order，盖在游戏 HUD 之上
		BossHealthBarWidget->AddToViewport(5);
	}
}

void AThunderFighterHUD::HideBossHealthBar()
{
	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->RemoveFromParent();
		BossHealthBarWidget = nullptr;
	}
}
