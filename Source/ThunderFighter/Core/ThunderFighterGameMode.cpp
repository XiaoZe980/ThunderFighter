// ThunderFighter - 雷霆战机 GameMode Implementation

#include "ThunderFighterGameMode.h"
#include "ThunderFighterPlayerController.h"
#include "Actors/EnemySpawner.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

AThunderFighterGameMode::AThunderFighterGameMode()
{
	// Allow ticking
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.0f;
}

void AThunderFighterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find enemy spawner in the level
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
			// Trigger game-over UI or level transition
			// Default: restart after delay (can be overridden in BP)
		}
	}
}

void AThunderFighterGameMode::AddScore(int32 Points)
{
	if (bGameOver) return;

	CurrentScore += Points;

	// Broadcast score changed event (can be bound by HUD)
	// Using a simple delegate — can be expanded later
}

void AThunderFighterGameMode::OnPlayerDefeated()
{
	if (bGameOver) return;

	bGameOver = true;
	GameOverTimer = GameOverDelay;

	// Stop enemy spawning
	if (EnemySpawnerRef.IsValid())
	{
		EnemySpawnerRef->StopSpawning();
	}

	// Notify via log
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player defeated! Final Score: %d"), CurrentScore);
}

void AThunderFighterGameMode::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AThunderFighterGameMode::ReturnToMainMenu()
{
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
