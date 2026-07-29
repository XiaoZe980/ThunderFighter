// ThunderFighter - 雷霆战机 EnemyBase Implementation

#include "EnemyBase.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/ProjectilePatternComponent.h"
#include "Core/ThunderFighterGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(30.0f, 30.0f, 5.0f));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(CollisionBox);

	// Mesh
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Health
	HealthComponent = CreateDefaultSubobject<UThunderFighterHealthComponent>(TEXT("HealthComponent"));

	// Projectile pattern (optional shooting)
	ProjectilePattern = CreateDefaultSubobject<UProjectilePatternComponent>(TEXT("ProjectilePattern"));

	// Tag this actor as enemy for projectile faction checks
	Tags.Add(TEXT("Enemy"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AliveTime = 0.0f;
	FireTimer = FireStartDelay;

	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleted.AddDynamic(this, &AEnemyBase::OnEnemyDefeated);
	}

	// Set up auto-fire if this enemy can shoot
	if (bCanShoot && ProjectilePattern)
	{
		// Auto-fire will start after FireStartDelay in Tick
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveTime += DeltaTime;
	ApplyMovement(DeltaTime);

	// Handle shooting
	if (bCanShoot && ProjectilePattern)
	{
		if (!bHasStartedShooting)
		{
			FireTimer -= DeltaTime;
			if (FireTimer <= 0.0f)
			{
				bHasStartedShooting = true;
				ProjectilePattern->SetAutoFire(true, FireInterval);
			}
		}
	}

	// Destroy if off-screen (moved too far)
	FVector Pos = GetActorLocation();
	if (FMath::Abs(Pos.X) > 3000.0f || FMath::Abs(Pos.Y) > 3000.0f)
	{
		Destroy();
	}
}

void AEnemyBase::Initialize(float InSpeed, float InHealth)
{
	BaseSpeed = InSpeed;

	if (HealthComponent)
	{
		HealthComponent->SetMaxHealth(InHealth, true);
	}
}

void AEnemyBase::OnEnemyDefeated()
{
	// Award score
	AThunderFighterGameMode* GM = Cast<AThunderFighterGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->AddScore(ScoreValue);
	}

	// Spawn pickup based on drop rate (handled by spawner or BP)

	// Disable collision and movement, then destroy
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (ProjectilePattern)
	{
		ProjectilePattern->SetAutoFire(false, 0.0f);
	}

	UE_LOG(LogTemp, Verbose, TEXT("[ThunderFighter] Enemy defeated: %s (+%d points)"), *GetName(), ScoreValue);

	// TODO: Play destruction VFX / animation before destroying
	Destroy();
}

void AEnemyBase::ApplyMovement(float DeltaTime)
{
	FVector Movement;

	if (MovementCurveX || MovementCurveY)
	{
		// Use curves for advanced movement patterns
		float OffsetX = 0.0f, OffsetY = 0.0f;

		if (MovementCurveX)
		{
			OffsetX = MovementCurveX->GetFloatValue(AliveTime);
		}
		if (MovementCurveY)
		{
			OffsetY = MovementCurveY->GetFloatValue(AliveTime);
		}

		// Base direction provides forward motion, curves provide lateral offsets
		FVector Forward = BaseDirection * BaseSpeed * MoveSpeedMultiplier;
		FVector Right = FVector(0.0f, 1.0f, 0.0f); // Lateral axis
		Movement = (Forward + Right * OffsetX + FVector(0.0f, 0.0f, 1.0f) * OffsetY) * DeltaTime;
	}
	else
	{
		// Simple linear movement
		Movement = BaseDirection * BaseSpeed * MoveSpeedMultiplier * DeltaTime;
	}

	AddActorWorldOffset(Movement, true);
}
