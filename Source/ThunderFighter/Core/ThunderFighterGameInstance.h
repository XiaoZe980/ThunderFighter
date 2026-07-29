// ThunderFighter - 雷霆战机 GameInstance
// Global game state management across levels

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ThunderFighterGameInstance.generated.h"

/**
 * GameInstance for ThunderFighter.
 * Stores persistent data across level transitions: high scores, settings, etc.
 */
UCLASS()
class THUNDERFIGHTER_API UThunderFighterGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	/** Get the highest score ever achieved */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Scoring")
	int32 GetHighScore() const { return HighScore; }

	/** Update the high score if the new score is higher */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Scoring")
	bool TryUpdateHighScore(int32 NewScore);

	/** Number of bombs the player has */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetBombCount() const { return BombCount; }

	/** Set bomb count */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetBombCount(int32 Count) { BombCount = Count; }

	/** Player lives remaining */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetLives() const { return Lives; }

	/** Set player lives */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetLives(int32 InLives) { Lives = InLives; }

	/** Current weapon level (persists across deaths) */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Gameplay")
	int32 GetWeaponLevel() const { return WeaponLevel; }

	/** Set weapon level */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void SetWeaponLevel(int32 Level) { WeaponLevel = FMath::Clamp(Level, 1, MaxWeaponLevel); }

	/** Reset all gameplay state for a new game */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Gameplay")
	void ResetGameState();

protected:
	/** Highest score */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Scoring")
	int32 HighScore = 0;

	/** Current bomb count */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 BombCount = 3;

	/** Player lives */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 Lives = 3;

	/** Current weapon level (1-5) */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Gameplay")
	int32 WeaponLevel = 1;

	/** Max weapon level */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Gameplay")
	int32 MaxWeaponLevel = 5;
};
