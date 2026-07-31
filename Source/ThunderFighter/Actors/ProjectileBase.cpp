// ThunderFighter - 雷霆战机 ProjectileBase 实现

#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ThunderFighterHealthComponent.h"

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

	// 打印所有碰撞到的对象，方便调试
	FString TargetTags;
	for (const FName& Tag : OtherActor->Tags) { TargetTags += Tag.ToString() + TEXT(" "); }
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] %s 碰到 %s (Tag: %s)"),
		*GetName(), *OtherActor->GetName(), *TargetTags);

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

		DeactivateProjectile();
	}
}

void AProjectileBase::DeactivateProjectile()
{
	// 目前：直接销毁。后续可添加对象池。
	Destroy();
}
