// ThunderFighter - 雷霆战机 PlayerController
// 通过 EnhancedInput 系统处理玩家输入

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThunderFighterPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 为 ThunderFighter 游戏绑定 EnhancedInput 动作的 PlayerController。
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
	// ---- 输入映射 ----

	/** 游戏玩法的主输入映射上下文 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputMappingContext> GameplayInputMapping;

	/** 水平移动（左/右） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_MoveHorizontal;

	/** 垂直移动（上/下） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_MoveVertical;

	/** 开火 / 射击动作 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Fire;

	/** 炸弹 / 特殊武器 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Bomb;

	/** 暂停动作 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Input")
	TObjectPtr<UInputAction> IA_Pause;

protected:
	// ---- 输入回调 ----

	/** 水平移动输入回调 */
	void OnMoveHorizontal(const FInputActionValue& Value);

	/** 垂直移动输入回调 */
	void OnMoveVertical(const FInputActionValue& Value);

	/** 开火开始回调 */
	void OnFireStarted(const FInputActionValue& Value);

	/** 开火停止回调 */
	void OnFireStopped(const FInputActionValue& Value);

	/** 炸弹激活回调 */
	void OnBomb(const FInputActionValue& Value);

	/** 暂停回调 */
	void OnPause(const FInputActionValue& Value);
};
