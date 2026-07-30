// ThunderFighter - 雷霆战机 HealthComponent
// 玩家和敌人通用的可复用生命值管理组件

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThunderFighterHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTaken, float, DamageAmount);

/**
 * 管理任意 Actor 的生命值、伤害和死亡逻辑的组件。
 * 附加到玩家战机、敌人或可破坏物体上使用。
 */
UCLASS(ClassGroup=(ThunderFighter), meta=(BlueprintSpawnableComponent))
class THUNDERFIGHTER_API UThunderFighterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThunderFighterHealthComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** 对该 Actor 造成伤害 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void TakeDamage(float DamageAmount);

	/** 恢复指定数量的生命值 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void Heal(float HealAmount);

	/** 此 Actor 是否仍然存活？ */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	bool IsAlive() const { return CurrentHealth > 0.0f; }

	/** 获取当前生命值百分比（0.0 - 1.0） */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetHealthPercent() const { return MaxHealth > 0.0f ? CurrentHealth / MaxHealth : 0.0f; }

	/** 获取当前生命值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetCurrentHealth() const { return CurrentHealth; }

	/** 获取最大生命值 */
	UFUNCTION(BlueprintPure, Category = "ThunderFighter|Health")
	float GetMaxHealth() const { return MaxHealth; }

	/** 设置最大生命值并可选择重置当前生命值 */
	UFUNCTION(BlueprintCallable, Category = "ThunderFighter|Health")
	void SetMaxHealth(float NewMaxHealth, bool bResetCurrentHealth = true);

	// ---- 委托 ----

	/** 生命值归零时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthDepleted OnHealthDepleted;

	/** 生命值变化时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnHealthChanged OnHealthChanged;

	/** 受到伤害时触发 */
	UPROPERTY(BlueprintAssignable, Category = "ThunderFighter|Health")
	FOnDamageTaken OnDamageTaken;

protected:
	/** 最大生命值 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ThunderFighter|Health")
	float MaxHealth = 100.0f;

	/** 当前生命值 */
	UPROPERTY(BlueprintReadOnly, Category = "ThunderFighter|Health")
	float CurrentHealth = 100.0f;

	/** 此 Actor 是否已死亡？（防止多次触发死亡事件） */
	bool bIsDead = false;
};
