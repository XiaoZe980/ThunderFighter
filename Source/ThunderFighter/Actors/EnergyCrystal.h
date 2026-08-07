// ThunderFighter - 雷霆战机 EnergyCrystal
// 经验球：敌人死亡掉落，玩家靠近时自动吸附飞向玩家

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnergyCrystal.generated.h"

class USphereComponent;
class UStaticMeshComponent;

/**
 * 经验球。敌人死亡时掉落，缓慢漂浮，玩家进入吸附范围后加速飞向玩家，
 * 触碰后给玩家添加经验。多个经验球叠加数量。
 */
UCLASS()
class THUNDERFIGHTER_API AEnergyCrystal : public AActor
{
	GENERATED_BODY()

public:
	AEnergyCrystal();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 ----

	/** 拾取触发球体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<USphereComponent> TriggerSphere;

	/** 视觉网格体 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> CrystalMesh;

public:
	/** 初始化经验球的经验值 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Experience")
	void Initialize(float InExpAmount);

	/** 获取经验值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Experience")
	float GetExpAmount() const { return ExpAmount; }

protected:
	/** 与玩家重叠时触发 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 此经验球提供的经验值 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Experience")
	float ExpAmount = 5.0f;

	/** 吸附开始距离（玩家进入此范围开始飞向玩家） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Experience")
	float MagnetRadius = 600.0f;

	/** 吸附飞行速度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Experience")
	float MagnetSpeed = 2500.0f;

	/** 存活时间（超时自动消失） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Experience")
	float Lifetime = 8.0f;

	/** 是否正在吸附 */
	bool bIsMagnetizing = false;

	/** 存活计时器 */
	float LifetimeTimer = 0.0f;
};
