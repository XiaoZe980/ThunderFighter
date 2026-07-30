// ThunderFighter - 雷霆战机 PlayerPawn Implementation

#include "ThunderFighterPlayerPawn.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/ThunderFighterWeaponComponent.h"
#include "Core/ThunderFighterGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

AThunderFighterPlayerPawn::AThunderFighterPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(20.0f, 20.0f, 5.0f));
	CollisionBox->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(CollisionBox);

	// Ship mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Health component
	HealthComponent = CreateDefaultSubobject<UThunderFighterHealthComponent>(TEXT("HealthComponent"));

	// Weapon component
	WeaponComponent = CreateDefaultSubobject<UThunderFighterWeaponComponent>(TEXT("WeaponComponent"));

	// Spring arm — holds camera above the player, looking down
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 3000.0f;                       // Height above the battlefield
	CameraSpringArm->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f)); // Pitch down for top-down view
	CameraSpringArm->bDoCollisionTest = false;                        // Don't clip against geometry
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->bInheritRoll = false;

	// Top-down camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	FollowCamera->SetFieldOfView(60.0f);
}

void AThunderFighterPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// Bind health depleted event
	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleted.AddDynamic(this, &AThunderFighterPlayerPawn::OnHealthDepleted);
	}
}

void AThunderFighterPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle invincibility timer
	if (bIsInvincible)
	{
		InvincibilityTimer -= DeltaTime;
		if (InvincibilityTimer <= 0.0f)
		{
			bIsInvincible = false;
		}
	}
}

void AThunderFighterPlayerPawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

	if (!WorldDirection.IsNearlyZero() && ScaleValue != 0.0f)
	{
		FVector Delta = WorldDirection * MoveSpeed * ScaleValue * GetWorld()->GetDeltaSeconds();
		AddActorWorldOffset(Delta, true);
		ClampToScreenBounds();
	}
}

void AThunderFighterPlayerPawn::StartFiring()
{
	bIsFiring = true;
	if (WeaponComponent)
	{
		WeaponComponent->StartFiring();
	}
}

void AThunderFighterPlayerPawn::StopFiring()
{
	bIsFiring = false;
	if (WeaponComponent)
	{
		WeaponComponent->StopFiring();
	}
}

void AThunderFighterPlayerPawn::UseBomb()
{
	if (WeaponComponent)
	{
		WeaponComponent->ActivateBomb();
	}
}

void AThunderFighterPlayerPawn::GrantInvincibility(float Duration)
{
	bIsInvincible = true;
	InvincibilityTimer = Duration;
}

void AThunderFighterPlayerPawn::ClampToScreenBounds()
{
	float MinX, MaxX, MinY, MaxY;
	GetScreenWorldBounds(MinX, MaxX, MinY, MaxY);

	FVector Pos = GetActorLocation();
	Pos.X = FMath::Clamp(Pos.X, MinX, MaxX);
	Pos.Y = FMath::Clamp(Pos.Y, MinY, MaxY);
	SetActorLocation(Pos);
}

void AThunderFighterPlayerPawn::GetScreenWorldBounds(float& OutMinX, float& OutMaxX, float& OutMinY, float& OutMaxY) const
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		OutMinX = OutMinY = -500.0f;
		OutMaxX = OutMaxY = 500.0f;
		return;
	}

	int32 ViewportX, ViewportY;
	PC->GetViewportSize(ViewportX, ViewportY);

	// Apply margin as a fraction of viewport edges
	const float MarginX = ViewportX * ScreenBoundaryMargin;
	const float MarginY = ViewportY * ScreenBoundaryMargin;

	// Deproject 4 corners of the screen (with margin) to find where they hit the player's Z plane
	auto DeprojectToPlane = [&](float ScreenX, float ScreenY) -> FVector2D
	{
		FVector Origin, Direction;
		PC->DeprojectScreenPositionToWorld(ScreenX, ScreenY, Origin, Direction);

		// Intersect ray with the horizontal plane at the player's Z
		float PlayerZ = GetActorLocation().Z;
		float T = (PlayerZ - Origin.Z) / Direction.Z;
		FVector HitPoint = Origin + Direction * T;
		return FVector2D(HitPoint.X, HitPoint.Y);
	};

	FVector2D TopLeft     = DeprojectToPlane(MarginX, MarginY);
	FVector2D TopRight    = DeprojectToPlane(ViewportX - MarginX, MarginY);
	FVector2D BottomLeft  = DeprojectToPlane(MarginX, ViewportY - MarginY);
	FVector2D BottomRight = DeprojectToPlane(ViewportX - MarginX, ViewportY - MarginY);

	OutMinX = FMath::Min({TopLeft.X, TopRight.X, BottomLeft.X, BottomRight.X});
	OutMaxX = FMath::Max({TopLeft.X, TopRight.X, BottomLeft.X, BottomRight.X});
	OutMinY = FMath::Min({TopLeft.Y, TopRight.Y, BottomLeft.Y, BottomRight.Y});
	OutMaxY = FMath::Max({TopLeft.Y, TopRight.Y, BottomLeft.Y, BottomRight.Y});
}

// Health depleted callback — to be wired up in BeginPlay
void AThunderFighterPlayerPawn::OnHealthDepleted()
{
	StopFiring();

	// Disable collision
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Notify GameMode
	if (AThunderFighterGameMode* GM = Cast<AThunderFighterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnPlayerDefeated();
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player ship destroyed!"));
}
