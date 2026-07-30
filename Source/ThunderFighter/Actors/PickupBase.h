// ThunderFighter - 雷霆战机 PickupBase
// 敌人掉落的强化道具基类

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UThunderFighterWeaponComponent;
class UThunderFighterHealthComponent;

/**
 * 拾取道具类型。
 */
UENUM(BlueprintType)
enum class EPickupType : uint8
{
	WeaponUpgrade	UMETA(DisplayName = "武器升级 (W)"),
	HealthRestore	UMETA(DisplayName = "生命恢复 (H)"),
	Bomb			UMETA(DisplayName = "炸弹 (B)"),
	ScoreBonus		UMETA(DisplayName = "分数奖励 (S)")
};

/**
 * 拾取道具 Actor。悬浮/向下漂移，被玩家收集时应用效果。
 */
UCLASS()
class THUNDERFIGHTER_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	APickupBase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 ----

	/** 用于检测拾取碰撞的触发球体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	/** 视觉网格体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

public:
	/** 使用类型和数值初始化此拾取道具 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Pickup")
	void Initialize(EPickupType InType, float InValue = 0.0f);

	/** 获取拾取道具类型 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Pickup")
	EPickupType GetPickupType() const { return PickupType; }

protected:
	/** 玩家与触发器重叠时调用 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 对玩家 Actor 应用拾取效果 */
	void ApplyEffect(AActor* PlayerActor);

	/** 此拾取道具的类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	EPickupType PickupType = EPickupType::WeaponUpgrade;

	/** 数值（例如生命值数量、分数点数、武器等级增量） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float Value = 1.0f;

	/** 浮动动画速度（上下抖动） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float FloatSpeed = 2.0f;

	/** 浮动幅度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float FloatAmplitude = 10.0f;

	/** 向下漂移速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float DriftSpeed = 100.0f;

	/** 自动销毁前的生命周期（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Pickup")
	float Lifetime = 10.0f;

private:
	/** 浮动动画的初始 Z 坐标 */
	float InitialZ = 0.0f;

	/** 生命周期累计值 */
	float LifetimeTimer = 0.0f;
};
