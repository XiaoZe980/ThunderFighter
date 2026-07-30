// ThunderFighter - 雷霆战机 HUD 实现

#include "ThunderFighterHUD.h"
#include "Blueprint/UserWidget.h"

AThunderFighterHUD::AThunderFighterHUD()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AThunderFighterHUD::BeginPlay()
{
	Super::BeginPlay();

	// 游戏开始时显示游戏玩法 HUD
	ShowGameplayHUD();
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
