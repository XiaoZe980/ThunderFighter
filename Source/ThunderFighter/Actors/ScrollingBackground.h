// ThunderFighter - 雷霆战机 ScrollingBackground
// Infinitely scrolling background for the shoot-em-up level

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScrollingBackground.generated.h"

class UStaticMeshComponent;

/**
 * Manages two background planes that scroll continuously and loop.
 * Place in level behind the gameplay area.
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

	// ---- Components ----

	/** Primary background plane */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> BackgroundPlane1;

	/** Secondary background plane (follows behind for seamless loop) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThunderFighter|Components")
	TObjectPtr<UStaticMeshComponent> BackgroundPlane2;

public:
	// ---- Configuration ----

	/** Scroll speed (units/sec). Positive = scroll downward. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	float ScrollSpeed = 200.0f;

	/** Direction of scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	FVector ScrollDirection = FVector(-1.0f, 0.0f, 0.0f);

	/** Length of each background plane (in scroll direction). When a plane moves past this offset, it loops. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	float PlaneLength = 2000.0f;

	/** Enable/disable scrolling */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThunderFighter|Background")
	bool bEnableScrolling = true;

protected:
	/** Update scroll position and handle looping */
	void ScrollPlanes(float DeltaTime);

	/** Offset between the two planes at start */
	FVector PlaneOffset;
};
