// ThunderFighter - 雷霆战机 HUD
// In-game heads-up display showing score, health, weapon level, bombs

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ThunderFighterHUD.generated.h"

class UUserWidget;

/**
 * HUD class for ThunderFighter.
 * Manages UMG widgets for in-game UI: score, health bar, weapon level, bombs, game-over screen.
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AThunderFighterHUD();

protected:
	virtual void BeginPlay() override;

public:
	/** Show the gameplay HUD */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowGameplayHUD();

	/** Hide the gameplay HUD */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void HideGameplayHUD();

	/** Show the game-over screen */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowGameOverScreen(int32 FinalScore, int32 HighScore);

	/** Show the pause menu */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void ShowPauseMenu();

	/** Hide the pause menu */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|UI")
	void HidePauseMenu();

	// ---- Blueprint Widget Classes ----

	/** Gameplay HUD widget class (score, health, etc.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> GameplayHUDClass;

	/** Game-over widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	/** Pause menu widget class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

protected:
	/** Currently active gameplay HUD widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> GameplayHUDWidget;

	/** Currently active game-over widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> GameOverWidget;

	/** Currently active pause menu widget instance */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|UI")
	TObjectPtr<UUserWidget> PauseMenuWidget;
};
