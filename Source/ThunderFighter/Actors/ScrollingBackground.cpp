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

	// 将第二个平面定位在第一个平面之后，并在 Z 轴上错开，避免 z-fighting 闪烁
	PlaneOffset = -ScrollDirection.GetSafeNormal() * PlaneLength;
	BackgroundPlane2->SetRelativeLocation(FVector(PlaneOffset.X, PlaneOffset.Y, ZSeparation));
}

void AScrollingBackground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bEnableScrolling)
	{
		ScrollPlanes(DeltaTime);
	}
}

void AScrollingBackground::ScrollPlanes(float DeltaTime)
{
	FVector Movement = ScrollDirection.GetSafeNormal() * ScrollSpeed * DeltaTime;

	// 移动两个平面
	BackgroundPlane1->AddWorldOffset(Movement, true);
	BackgroundPlane2->AddWorldOffset(Movement, true);

	FVector Plane1Pos = BackgroundPlane1->GetComponentLocation();
	FVector Plane2Pos = BackgroundPlane2->GetComponentLocation();

	FVector ScrollDir = ScrollDirection.GetSafeNormal();
	float LoopThreshold = PlaneLength;

	// 检查 Plane1 是否已滚动超过循环点
	float Plane1Dot = FVector::DotProduct(Plane1Pos, ScrollDir);
	float Plane2Dot = FVector::DotProduct(Plane2Pos, ScrollDir);

	// 若 Plane1 点积更大（沿滚动方向更远）则 Plane1 位于"前方"
	// 循环跳转时只调整 X/Y，保留各平面自己的 Z，避免 z-fighting
	auto RepositionBehind = [](UStaticMeshComponent* PlaneToMove, UStaticMeshComponent* PlaneAhead, const FVector& Offset)
	{
		FVector NewPos = PlaneToMove->GetComponentLocation();
		NewPos.X = PlaneAhead->GetComponentLocation().X + Offset.X;
		NewPos.Y = PlaneAhead->GetComponentLocation().Y + Offset.Y;
		PlaneToMove->SetWorldLocation(NewPos);
	};

	if (ScrollSpeed > 0.0f)
	{
		// 向前滚动：Plane1 在前
		if (Plane2Dot < Plane1Dot - LoopThreshold)
		{
			// Plane2 落后太多，将其移到 Plane1 前方
			RepositionBehind(BackgroundPlane2, BackgroundPlane1, ScrollDir * LoopThreshold);
		}
		else if (Plane1Dot < Plane2Dot - LoopThreshold)
		{
			// Plane1 落后太多，将其移到 Plane2 前方
			RepositionBehind(BackgroundPlane1, BackgroundPlane2, ScrollDir * LoopThreshold);
		}
	}
	else
	{
		// 向后滚动
		if (Plane2Dot > Plane1Dot + LoopThreshold)
		{
			RepositionBehind(BackgroundPlane2, BackgroundPlane1, -ScrollDir * LoopThreshold);
		}
		else if (Plane1Dot > Plane2Dot + LoopThreshold)
		{
			RepositionBehind(BackgroundPlane1, BackgroundPlane2, -ScrollDir * LoopThreshold);
		}
	}
}
