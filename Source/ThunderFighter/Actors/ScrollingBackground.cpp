// ThunderFighter - 雷霆战机 ScrollingBackground 实现

#include "ScrollingBackground.h"
#include "Components/StaticMeshComponent.h"

AScrollingBackground::AScrollingBackground()
{
	PrimaryActorTick.bCanEverTick = true;

	BackgroundPlane1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundPlane1"));
	BackgroundPlane1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BackgroundPlane1);

	BackgroundPlane2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundPlane2"));
	BackgroundPlane2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BackgroundPlane2->SetupAttachment(RootComponent);
}

void AScrollingBackground::BeginPlay()
{
	Super::BeginPlay();

	// 记录 Plane1 初始世界位置，作为滚动基准点
	InitialPlane1Pos = BackgroundPlane1->GetComponentLocation();

	// 按配置自动缩放平面（Plane mesh 默认 100x100 单位，X=PlaneLength, Y=PlaneWidth）
	const FVector PlaneScale(PlaneLength / 100.0f, PlaneWidth / 100.0f, 1.0f);
	BackgroundPlane1->SetRelativeScale3D(PlaneScale);
	BackgroundPlane2->SetRelativeScale3D(PlaneScale);

	UE_LOG(LogTemp, Log, TEXT("[Background] PlaneLength=%.0f PlaneWidth=%.0f Scale=(%.1f, %.1f, 1)"),
		PlaneLength, PlaneWidth, PlaneScale.X, PlaneScale.Y);
}

void AScrollingBackground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 诊断：打印前几次 Tick 的滚动值，确认滚动生效
	if (DebugLogCount < 5)
	{
		UE_LOG(LogTemp, Log, TEXT("[Background] Tick ScrollSpeed=%.1f ScrollDistance=%.1f DeltaTime=%.4f bEnable=%d"),
			ScrollSpeed, ScrollDistance, DeltaTime, bEnableScrolling ? 1 : 0);
		DebugLogCount++;
	}

	if (!bEnableScrolling) return;
	if (PlaneLength <= 0.0f) return;

	// 累计滚动距离，并对 PlaneLength 取模实现无缝循环
	ScrollDistance += ScrollSpeed * DeltaTime;
	ScrollDistance = FMath::Fmod(ScrollDistance, PlaneLength);

	const FVector Dir = ScrollDirection.GetSafeNormal();

	// Plane1 沿滚动方向从基准点移动 ScrollDistance
	FVector P1 = InitialPlane1Pos + Dir * ScrollDistance;

	// Plane2 位于 Plane1 沿滚动方向的前方 PlaneLength 处（衔接）
	FVector P2 = P1 - Dir * PlaneLength;
	P2.Z = P1.Z + ZSeparation; // Z 轴错开，避免 z-fighting

	BackgroundPlane1->SetWorldLocation(P1);
	BackgroundPlane2->SetWorldLocation(P2);
}
