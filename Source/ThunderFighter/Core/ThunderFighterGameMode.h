// ThunderFighter - 雷霆战机 GameMode
// Manages game rules, scoring, wave control, and game state transitions

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThunderFighterGameMode.generated.h"

class AEnemySpawner;

/**
 * GameMode for ThunderFighter.
 * Controls wave spawning, scoring, and game-over conditions.
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThunderFighterGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Add score for the player */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Scoring")
	void AddScore(int32 Points);

	/** Get current score */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Scoring")
	int32 GetScore() const { return CurrentScore; }

	/** Player has been defeated — trigger game over */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void OnPlayerDefeated();

	/** Restart the current level */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void RestartGame();

	/** Return to main menu */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|GameState")
	void ReturnToMainMenu();

protected:
	/** Score accumulated this session */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Scoring")
	int32 CurrentScore = 0;

	/** Is the game currently over? */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|GameState")
	bool bGameOver = false;

	/** Time in seconds before game-over actions trigger */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|GameState")
	float GameOverDelay = 3.0f;

	/** Timer handle for game-over delay */
	float GameOverTimer = 0.0f;

	/** Reference to the enemy spawner in the level */
	UPROPERTY()
	TWeakObjectPtr<AEnemySpawner> EnemySpawnerRef;

	/** Name of the main menu level */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Levels")
	FName MainMenuLevelName = TEXT("MainMenu");
};
