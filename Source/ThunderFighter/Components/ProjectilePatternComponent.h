// ThunderFighter - 雷霆战机 ProjectilePatternComponent
// 敌人和 Boss 的可配置子弹模式发射器

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/ProjectileBase.h"
#include "ProjectilePatternComponent.generated.h"

/**
 * 附加到敌人 Actor 上以定义其子弹发射模式。
 * 支持多种模式类型：单发、散射、圆形、螺旋。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UProjectilePatternComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectilePatternComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 沿正前方发射单发弹幕 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireSingle(FVector Direction, float Speed, float Damage);

	/** 发射扇形散射弹幕 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireSpread(int32 Count, float SpreadAngle, float Speed, float Damage);

	/** 发射圆形弹幕 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireCircle(int32 Count, float Speed, float Damage, float StartAngle = 0.0f);

	/** 向目标位置发射（瞄准射击） */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireAtTarget(FVector TargetLocation, float Speed, float Damage, float Inaccuracy = 0.0f);

	/** 向玩家发射 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void FireAtPlayer(float Speed, float Damage, float Inaccuracy = 0.0f);

	/** 启用/禁用自动射击 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetAutoFire(bool bEnable, float FireInterval);

	/** 设置要生成的弹幕类型 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Projectile")
	void SetProjectileClass(TSubclassOf<AProjectileBase> InClass) { ProjectileClass = InClass; }

protected:
	/** 生成单个弹幕 */
	AProjectileBase* SpawnProjectile(FVector Direction, float Speed, float Damage);

	/** 生成弹幕的阵营 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	EProjectileFaction Faction = EProjectileFaction::Enemy;

	/** 要生成的弹幕类型 */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** 每次射击的随机散布（度） */
	UPROPERTY(EditDefaultsOnly, Category = "ThunderFighter|Projectile")
	float BaseInaccuracy = 0.0f;

	/** 自动射击子弹速度 */
	UPROPERTY(EditAnywhere, Category = "ThunderFighter|Projectile|AutoFire")
	float AutoFireSpeed = 800.0f;

	/** 自动射击子弹伤害 */
	UPROPERTY(EditAnywhere, Category = "ThunderFighter|Projectile|AutoFire")
	float AutoFireDamage = 10.0f;

private:
	/** 自动射击状态 */
	bool bAutoFire = false;

	/** 自动射击间隔 */
	float AutoFireInterval = 0.5f;

	/** 自动射击计时器 */
	float AutoFireTimer = 0.0f;
};
