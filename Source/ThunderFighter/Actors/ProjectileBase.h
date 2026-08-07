// ThunderFighter - 雷霆战机 ProjectileBase
// 所有弹幕的基类（玩家和敌方子弹）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

/**
 * 弹幕的阵营标识。
 * 决定弹幕可以对哪些 Actor 造成伤害。
 */
UENUM(BlueprintType)
enum class EProjectileFaction : uint8
{
	Player	UMETA(DisplayName = "玩家"),
	Enemy	UMETA(DisplayName = "敌方")
};

/**
 * 弹幕基类。以恒定速度直线移动。
 * 碰撞时可对敌对阵营的 Actor 造成伤害。
 */
UCLASS()
class THUNDERFIGHTER_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	virtual void Tick(float DeltaTime) override;

	/**
	 * 生成后初始化弹幕。
	 * @param InSpeed 移动速度（单位/秒）
	 * @param InDamage 命中时造成的伤害
	 * @param InFaction 此弹幕所属的阵营
	 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void Initialize(float InSpeed, float InDamage, EProjectileFaction InFaction);

	/** 获取此弹幕的阵营 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Projectile")
	EProjectileFaction GetFaction() const { return Faction; }

	/** 获取伤害值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Projectile")
	float GetDamage() const { return Damage; }

	/** 设置弹幕生命周期（此秒数后自动销毁） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetLifetime(float InLifetime);

	/** 设置穿透：命中敌人不销毁，继续飞行 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetPiercing(bool bEnable);

	/** 设置追踪：朝最近敌人转向 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetHoming(bool bEnable);

	/** 设置弹射次数：命中后转向最近敌人 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetBounceCount(int32 Count);

protected:
	virtual void BeginPlay() override;

	/** 查找最近的敌对目标（追踪/弹射用） */
	AActor* FindNearestEnemy();

	/** 命中后转向最近敌人（弹射） */
	void BounceToNearestEnemy();

	/** 弹幕命中物体时调用（碰撞检测，用于有碰撞响应的场景） */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** 弹幕与物体重叠时调用（Overlap 模式，用于子弹与敌人的穿透检测） */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 停用此弹幕并返回对象池（或销毁） */
	void DeactivateProjectile();

	/** 处理命中/重叠事件的公共逻辑 */
	void HandleHit(AActor* OtherActor);

	// ---- 组件 ----

	/** 球体碰撞体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<USphereComponent> CollisionSphere;

	/** 视觉网格体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	/** 弹幕移动组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	// ---- 属性 ----

	/** 弹幕速度 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	float Speed = 2000.0f;

	/** 命中时造成的伤害 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	float Damage = 10.0f;

	/** 此弹幕所属的阵营 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Projectile")
	EProjectileFaction Faction = EProjectileFaction::Player;

	/** 自动销毁前的生命周期（0 = 无限，由外部管理） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	float MaxLifetime = 5.0f;

	/** 当前生命周期累计值 */
	float LifetimeTimer = 0.0f;

	/** 是否穿透（命中不销毁，只对同一敌人伤害一次） */
	bool bPiercing = false;

	/** 是否追踪（朝最近敌人转向） */
	bool bHoming = false;

	/** 剩余弹射次数 */
	int32 BounceCountRemaining = 0;

	/** 已命中过的目标（穿透/弹射时避免重复伤害） */
	TSet<AActor*> HitTargets;
};
