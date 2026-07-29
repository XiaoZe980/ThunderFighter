// ThunderFighter - 雷霆战机 ProjectileBase Implementation

#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ThunderFighterHealthComponent.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision sphere
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(10.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionSphere);

	// Visual mesh
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Projectile movement
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

	LifetimeTimer = 0.0f;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Lifetime check
	if (MaxLifetime > 0.0f)
	{
		LifetimeTimer += DeltaTime;
		if (LifetimeTimer >= MaxLifetime)
		{
			DeactivateProjectile();
		}
	}
}

void AProjectileBase::Initialize(float InSpeed, float InDamage, EProjectileFaction InFaction)
{
	Speed = InSpeed;
	Damage = InDamage;
	Faction = InFaction;

	if (MovementComponent)
	{
		MovementComponent->InitialSpeed = Speed;
		MovementComponent->MaxSpeed = Speed;
	}

	LifetimeTimer = 0.0f;
}

void AProjectileBase::SetLifetime(float InLifetime)
{
	MaxLifetime = InLifetime;
	LifetimeTimer = 0.0f;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// Check faction tag on the other actor
	bool bIsEnemy = OtherActor->ActorHasTag(Faction == EProjectileFaction::Player ? TEXT("Enemy") : TEXT("Player"));

	if (bIsEnemy)
	{
		// Apply damage if the other actor has a health component
		UThunderFighterHealthComponent* HealthComp = OtherActor->FindComponentByClass<UThunderFighterHealthComponent>();
		if (HealthComp)
		{
			HealthComp->TakeDamage(Damage);
		}

		DeactivateProjectile();
	}
}

void AProjectileBase::DeactivateProjectile()
{
	// For now: simply destroy. Object pooling can be added later.
	Destroy();
}
