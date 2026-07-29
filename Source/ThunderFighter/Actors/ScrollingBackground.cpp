// ThunderFighter - 雷霆战机 ScrollingBackground Implementation

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

	// Position the second plane to follow the first
	PlaneOffset = -ScrollDirection.GetSafeNormal() * PlaneLength;
	BackgroundPlane2->SetRelativeLocation(PlaneOffset);
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

	// Move both planes
	BackgroundPlane1->AddWorldOffset(Movement, true);
	BackgroundPlane2->AddWorldOffset(Movement, true);

	FVector Plane1Pos = BackgroundPlane1->GetComponentLocation();
	FVector Plane2Pos = BackgroundPlane2->GetComponentLocation();

	FVector ScrollDir = ScrollDirection.GetSafeNormal();
	float LoopThreshold = PlaneLength;

	// Check if Plane1 has scrolled past the loop point
	float Plane1Dot = FVector::DotProduct(Plane1Pos, ScrollDir);
	float Plane2Dot = FVector::DotProduct(Plane2Pos, ScrollDir);

	// Plane1 is "ahead" if its dot product is larger (further along scroll direction)
	if (ScrollSpeed > 0.0f)
	{
		// Scrolling forward: Plane1 is ahead
		if (Plane2Dot < Plane1Dot - LoopThreshold)
		{
			// Plane2 is too far behind, move it ahead of Plane1
			BackgroundPlane2->SetWorldLocation(Plane1Pos + ScrollDir * LoopThreshold);
		}
		else if (Plane1Dot < Plane2Dot - LoopThreshold)
		{
			// Plane1 is too far behind, move it ahead of Plane2
			BackgroundPlane1->SetWorldLocation(Plane2Pos + ScrollDir * LoopThreshold);
		}
	}
	else
	{
		// Scrolling backward
		if (Plane2Dot > Plane1Dot + LoopThreshold)
		{
			BackgroundPlane2->SetWorldLocation(Plane1Pos - ScrollDir * LoopThreshold);
		}
		else if (Plane1Dot > Plane2Dot + LoopThreshold)
		{
			BackgroundPlane1->SetWorldLocation(Plane2Pos - ScrollDir * LoopThreshold);
		}
	}
}
