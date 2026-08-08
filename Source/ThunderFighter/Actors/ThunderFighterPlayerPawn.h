// ThunderFighter - 雷霆战机 PlayerPawn
// 玩家控制的战斗机

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ThunderFighterPlayerPawn.generated.h"

class UThunderFighterHealthComponent;
class UThunderFighterWeaponComponent;
class UPlayerLevelComponent;
class USpringArmComponent;
class UCameraComponent;
class UBoxComponent;

/**
 * 玩家战斗机 Pawn。通过 PlayerController 的 EnhancedInput 控制。
 * 处理屏幕边界内的移动、自动射击和炸弹使用。
 */
UCLASS()
class THUNDERFIGHTER_API AThunderFighterPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AThunderFighterPlayerPawn();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 ----

	/** 玩家战机的根碰撞盒 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UBoxComponent> CollisionBox;

	/** 玩家战机的静态网格体或精灵 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> ShipMesh;

	/** 生命值组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterHealthComponent> HealthComponent;

	/** 武器组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UThunderFighterWeaponComponent> WeaponComponent;

	/** 经验/等级组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UPlayerLevelComponent> LevelComponent;

	/** 相机底座：X 跟随玩家（前后），Y 固定在 0（横向移动时相机不跟随） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Camera")
	TObjectPtr<USceneComponent> CameraRig;

	/** 俯视摄像机的弹簧臂 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Camera")
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	/** 俯视摄像机 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

public:
	// ---- 移动 ----

	/** 添加世界空间移动输入（限制在屏幕边界内） */
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	/** 移动速度（单位/秒） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float MoveSpeed = 1200.0f;

	/** 屏幕边界边距（视口百分比，0.0-0.5） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement")
	float ScreenBoundaryMargin = 0.05f;

	/** 地图前边界（X 轴最小，玩家前后移动的最远后退位置） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement|Map")
	float MapBoundMinX = -3000.0f;

	/** 地图后边界（X 轴最大，玩家前后移动的最远前进位置） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Movement|Map")
	float MapBoundMaxX = 3000.0f;

	// ---- 射击 ----

	/** 开始自动射击 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void StartFiring();

	/** 停止自动射击 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void StopFiring();

	/** 使用炸弹（清屏攻击） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void UseBomb();

	/** 玩家当前是否在射击？ */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Combat")
	bool IsFiring() const { return bIsFiring; }

	/** 若为 true，玩家自动射击（STG 游戏默认行为） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	bool bAutoFireEnabled = true;

	/** 与敌人碰撞时受到的伤害 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float CollisionDamage = 20.0f;

	/** 受伤后的无敌时长（秒，不屈强化可提升） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ThunderFighter|Combat")
	float InvincibilityDuration = 1.0f;

	// ---- 无敌 ----

	/** 授予临时无敌状态（例如受到伤害后） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Combat")
	void GrantInvincibility(float Duration);

	/** 玩家当前是否处于无敌状态？ */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Combat")
	bool IsInvincible() const { return bIsInvincible; }

protected:
	/** 生命值归零时调用 */
	UFUNCTION()
	void OnHealthDepleted();

	/** 被敌人或敌方子弹碰撞时调用 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 将玩家位置限制在屏幕边界内 */
	void ClampToScreenBounds();

	/** 获取世界空间中的屏幕边界 */
	void GetScreenWorldBounds(float& OutMinX, float& OutMaxX, float& OutMinY, float& OutMaxY) const;

	/** 应用局外永久升级（从 GameInstance 读取，进关卡时调用） */
	void ApplyPermanentUpgrades();

	/** 射击状态 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Combat")
	bool bIsFiring = false;

	/** 无敌标志 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Combat")
	bool bIsInvincible = false;

	/** 无敌计时器 */
	float InvincibilityTimer = 0.0f;
};
