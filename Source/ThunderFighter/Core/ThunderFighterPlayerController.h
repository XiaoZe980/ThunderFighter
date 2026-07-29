// ThunderFighter - 雷霆战机 PlayerController
// Handles player input via EnhancedInput system

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThunderFighterPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * PlayerController that binds EnhancedInput actions for the ThunderFighter game.
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AThunderFighterPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	// ---- Input Mappings ----

	/** Main input mapping context for gameplay */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputMappingContext> GameplayInputMapping;

	/** Horizontal movement (left/right) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_MoveHorizontal;

	/** Vertical movement (up/down) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_MoveVertical;

	/** Fire / shoot action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Fire;

	/** Bomb / special weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Bomb;

	/** Pause action */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Pause;

protected:
	// ---- Input Callbacks ----

	/** Horizontal movement input callback */
	void OnMoveHorizontal(const FInputActionValue& Value);

	/** Vertical movement input callback */
	void OnMoveVertical(const FInputActionValue& Value);

	/** Fire started callback */
	void OnFireStarted(const FInputActionValue& Value);

	/** Fire stopped callback */
	void OnFireStopped(const FInputActionValue& Value);

	/** Bomb activation callback */
	void OnBomb(const FInputActionValue& Value);

	/** Pause callback */
	void OnPause(const FInputActionValue& Value);
};
