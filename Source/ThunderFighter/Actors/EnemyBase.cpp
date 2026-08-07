// ThunderFighter - 雷霆战机 EnemyBase 实现

#include "EnemyBase.h"
#include "EnergyCrystal.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/ProjectilePatternComponent.h"
#include "Components/PickupSpawnComponent.h"
#include "Core/ThunderFighterGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 碰撞体
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(30.0f, 30.0f, 5.0f));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(CollisionBox);

	// 网格体
	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 生命值
	HealthComponent = CreateDefaultSubobject<UThunderFighterHealthComponent>(TEXT("HealthComponent"));

	// 弹幕模式（可选射击）
	ProjectilePattern = CreateDefaultSubobject<UProjectilePatternComponent>(TEXT("ProjectilePattern"));

	// 道具掉落
	PickupSpawn = CreateDefaultSubobject<UPickupSpawnComponent>(TEXT("PickupSpawn"));

	// 将此 Actor 标记为敌方，用于弹幕阵营检查
	Tags.Add(TEXT("Enemy"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AliveTime = 0.0f;
	FireTimer = FireStartDelay;

	FVector Pos = GetActorLocation();
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] 敌人生成: %s 位置=(%.0f, %.0f, %.0f)"),
		*GetName(), Pos.X, Pos.Y, Pos.Z);

	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleted.AddDynamic(this, &AEnemyBase::OnEnemyDefeated);
	}

	// 如果此敌人可以射击则设置自动开火
	if (bCanShoot && ProjectilePattern)
	{
		// 自动开火将在 Tick 中于 FireStartDelay 后开始
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AliveTime += DeltaTime;
	ApplyMovement(DeltaTime);

	// 处理射击
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

	// 移出屏幕（移动太远）时销毁
	FVector Pos = GetActorLocation();
	if (FMath::Abs(Pos.X) > 3000.0f || FMath::Abs(Pos.Y) > 3000.0f)
	{
		Destroy();
	}
}

void AEnemyBase::Initialize(float InSpeed, float InHealth)
{
	// 只覆盖显式传入的值；<=0 表示使用蓝图配置的默认值
	if (InSpeed > 0.0f)
	{
		BaseSpeed = InSpeed;
	}

	if (InHealth > 0.0f && HealthComponent)
	{
		HealthComponent->SetMaxHealth(InHealth, true);
	}
}

void AEnemyBase::OnEnemyDefeated()
{
	// 奖励分数
	AThunderFighterGameMode* GM = Cast<AThunderFighterGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->AddScore(ScoreValue);
	}

	// 按照 DropRate 概率掉落道具
	if (PickupSpawn)
	{
		PickupSpawn->TrySpawnPickup(GetActorLocation(), DropRate);
	}

	// 掉落经验球（Roguelike 成长循环）
	if (EnergyCrystalClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AEnergyCrystal* Crystal = GetWorld()->SpawnActor<AEnergyCrystal>(
			EnergyCrystalClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);

		if (Crystal)
		{
			Crystal->Initialize(ExpValue);
		}
	}

	// 禁用碰撞和移动，然后销毁
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (ProjectilePattern)
	{
		ProjectilePattern->SetAutoFire(false, 0.0f);
	}

	UE_LOG(LogTemp, Verbose, TEXT("[ThunderFighter] Enemy defeated: %s (+%d points)"), *GetName(), ScoreValue);

	// TODO: 销毁前播放爆炸 VFX / 动画
	Destroy();
}

void AEnemyBase::ApplyMovement(float DeltaTime)
{
	FVector Movement;

	if (MovementCurveX || MovementCurveY)
	{
		// 使用曲线实现高级移动模式
		float OffsetX = 0.0f, OffsetY = 0.0f;

		if (MovementCurveX)
		{
			OffsetX = MovementCurveX->GetFloatValue(AliveTime);
		}
		if (MovementCurveY)
		{
			OffsetY = MovementCurveY->GetFloatValue(AliveTime);
		}

		// 基础方向提供前进运动，曲线提供横向偏移
		FVector Forward = BaseDirection * BaseSpeed * MoveSpeedMultiplier;
		FVector Right = FVector(0.0f, 1.0f, 0.0f); // 横向轴
		Movement = (Forward + Right * OffsetX + FVector(0.0f, 0.0f, 1.0f) * OffsetY) * DeltaTime;
	}
	else
	{
		// 简单直线移动
		Movement = BaseDirection * BaseSpeed * MoveSpeedMultiplier * DeltaTime;
	}

	AddActorWorldOffset(Movement, true);
}
