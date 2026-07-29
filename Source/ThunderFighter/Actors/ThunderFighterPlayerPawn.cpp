// ThunderFighter - 雷霆战机 PlayerPawn Implementation

#include "ThunderFighterPlayerPawn.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/ThunderFighterWeaponComponent.h"
#include "Core/ThunderFighterGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
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

	FVector WorldOrigin, WorldDirection;
	// Deproject screen edge to world
	PC->DeprojectScreenPositionToWorld(0.0f, 0.0f, WorldOrigin, WorldDirection);
	// Approximate world bounds at the player's Z level
	float Z = GetActorLocation().Z;

	// Simplified: use a percentage-based approach
	const float HalfWidth = ViewportX * 0.5f * (1.0f - ScreenBoundaryMargin);
	const float HalfHeight = ViewportY * 0.5f * (1.0f - ScreenBoundaryMargin);

	// Convert screen units to world units using a rough scaling factor
	const float WorldScale = 0.05f; // tune this per-project
	OutMinX = -HalfWidth * WorldScale;
	OutMaxX = HalfWidth * WorldScale;
	OutMinY = -HalfHeight * WorldScale;
	OutMaxY = HalfHeight * WorldScale;
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
