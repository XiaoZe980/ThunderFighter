// ThunderFighter - 雷霆战机 ProjectileBase 实现

#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "EngineUtils.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 碰撞球体
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(10.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionSphere);

	// 视觉网格体
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 弹幕移动组件
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
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

	LifetimeTimer = 0.0f;
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 生命周期检查
	if (MaxLifetime > 0.0f)
	{
		LifetimeTimer += DeltaTime;
		if (LifetimeTimer >= MaxLifetime)
		{
			DeactivateProjectile();
		}
	}

	// 追踪：朝最近敌人平滑转向
	if (bHoming)
	{
		AActor* Target = FindNearestEnemy();
		if (Target && MovementComponent)
		{
			FVector DirToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector DesiredVelocity = DirToTarget * Speed;
			// 平滑转向，转向速率 600 度/秒左右（用插值近似）
			MovementComponent->Velocity = FMath::VInterpConstantTo(
				MovementComponent->Velocity, DesiredVelocity, DeltaTime, 1000.0f);
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

void AProjectileBase::SetPiercing(bool bEnable)
{
	bPiercing = bEnable;
}

void AProjectileBase::SetHoming(bool bEnable)
{
	bHoming = bEnable;
}

void AProjectileBase::SetBounceCount(int32 Count)
{
	BounceCountRemaining = Count;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HandleHit(OtherActor);
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HandleHit(OtherActor);
}

void AProjectileBase::HandleHit(AActor* OtherActor)
{
	if (!OtherActor || OtherActor == GetOwner()) return;

	// 穿透/弹射时避免对同一目标重复伤害
	if (HitTargets.Contains(OtherActor)) return;

	// 检查另一个 Actor 的阵营标签
	FName RequiredTag = Faction == EProjectileFaction::Player ? TEXT("Enemy") : TEXT("Player");
	bool bIsEnemy = OtherActor->ActorHasTag(RequiredTag);

	if (bIsEnemy)
	{
		UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] %s 命中敌对阵营 %s (伤害: %.1f)"),
			*GetName(), *OtherActor->GetName(), Damage);

		// 如果另一个 Actor 有生命值组件则对其造成伤害
		UThunderFighterHealthComponent* HealthComp = OtherActor->FindComponentByClass<UThunderFighterHealthComponent>();
		if (HealthComp)
		{
			HealthComp->TakeDamage(Damage);
		}

		HitTargets.Add(OtherActor);

		// 穿透：不销毁，继续飞行
		if (bPiercing)
		{
			return;
		}

		// 弹射：转向最近敌人继续飞行
		if (BounceCountRemaining > 0)
		{
			BounceCountRemaining--;
			BounceToNearestEnemy();
			return;
		}

		DeactivateProjectile();
	}
}

AActor* AProjectileBase::FindNearestEnemy()
{
	AActor* Nearest = nullptr;
	float NearestDistSq = FLT_MAX;
	FName RequiredTag = Faction == EProjectileFaction::Player ? TEXT("Enemy") : TEXT("Player");

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor || !Actor->ActorHasTag(RequiredTag)) continue;
		if (HitTargets.Contains(Actor)) continue;

		float DistSq = FVector::DistSquared(GetActorLocation(), Actor->GetActorLocation());
		if (DistSq < NearestDistSq)
		{
			NearestDistSq = DistSq;
			Nearest = Actor;
		}
	}

	return Nearest;
}

void AProjectileBase::BounceToNearestEnemy()
{
	AActor* Target = FindNearestEnemy();
	if (Target && MovementComponent)
	{
		FVector Dir = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		MovementComponent->Velocity = Dir * Speed;
	}
	else
	{
		// 没有可弹射的目标，销毁
		DeactivateProjectile();
	}
}

void AProjectileBase::DeactivateProjectile()
{
	// 目前：直接销毁。后续可添加对象池。
	Destroy();
}
