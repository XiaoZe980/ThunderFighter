// ThunderFighter - 雷霆战机 ScrollingBackground
// 射击游戏关卡中无限滚动的背景

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScrollingBackground.generated.h"

class UStaticMeshComponent;

/**
 * 管理两个连续滚动并循环的背景平面。
 * 放置在游戏区域后方的关卡中。
 */
UCLASS()
class THUNDERFIGHTER_API AScrollingBackground : public AActor
{
	GENERATED_BODY()

public:
	AScrollingBackground();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// ---- 组件 ----

	/** 主背景平面 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> BackgroundPlane1;

	/** 副背景平面（跟随主平面后方以实现无缝循环） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> BackgroundPlane2;

public:
	// ---- 配置 ----

	/** 滚动速度（单位/秒）。正值 = 向下滚动。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	float ScrollSpeed = 200.0f;

	/** 滚动方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	FVector ScrollDirection = FVector(-1.0f, 0.0f, 0.0f);

	/** 每个背景平面的长度（沿滚动方向）。当平面移动超过此偏移时循环。0 = 自动计算。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	float PlaneLength = 0.0f;

	/** 两个平面之间的 Z 轴间隔，避免 z-fighting 闪烁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	float ZSeparation = 10.0f;

	/** 启用/禁用滚动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	bool bEnableScrolling = true;

protected:
	/** 更新滚动位置并处理循环 */
	void ScrollPlanes(float DeltaTime);

	/** 两个平面之间的初始偏移 */
	FVector PlaneOffset;
};
