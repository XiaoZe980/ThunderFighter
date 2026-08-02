// ThunderFighter - 雷霆战机 BossEnemy 实现

#include "BossEnemy.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Actors/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy::ABossEnemy()
{
	// Boss 体型更大
	if (CollisionBox)
	{
		CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 20.0f));
	}

	// Boss 默认高分值，蓝图可覆盖
	ScoreValue = 5000;
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	// 提供默认 3 阶段配置（蓝图可完全覆盖）
	if (Phases.Num() == 0)
	{
		FEnemyPhase Phase1;
		Phase1.HealthThreshold = 1.0f;
		Phase1.PatternType = EBossPatternType::Aimed;
		Phase1.FireInterval = 1.5f;
		Phase1.BulletSpeed = 700.0f;
		Phases.Add(Phase1);

		FEnemyPhase Phase2;
		Phase2.HealthThreshold = 0.66f;
		Phase2.PatternType = EBossPatternType::Spread;
		Phase2.FireInterval = 1.2f;
		Phase2.BulletCount = 5;
		Phase2.SpreadAngle = 60.0f;
		Phase2.BulletSpeed = 650.0f;
		Phases.Add(Phase2);

		FEnemyPhase Phase3;
		Phase3.HealthThreshold = 0.33f;
		Phase3.PatternType = EBossPatternType::Circle;
		Phase3.FireInterval = 1.0f;
		Phase3.BulletCount = 12;
		Phase3.BulletSpeed = 600.0f;
		Phases.Add(Phase3);
	}

	// 记录扫射基准位置
	HoverYOrigin = GetActorLocation().Y;

	// 绑定血量变化以触发阶段切换
	if (HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ABossEnemy::OnBossHealthChanged);
	}
}

void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasEntered) return;

	// Boss 专属射击节奏
	BossFireTimer -= DeltaTime;
	if (BossFireTimer <= 0.0f)
	{
		FireBurst();
		if (Phases.Num() > 0)
		{
			int32 SafeIndex = FMath::Clamp(CurrentPhaseIndex, 0, Phases.Num() - 1);
			BossFireTimer = Phases[SafeIndex].FireInterval;
		}
	}
}

void ABossEnemy::ApplyMovement(float DeltaTime)
{
	if (!bHasEntered)
	{
		// 入场阶段：沿 +X 进入战场，到达 HoverX 后停住
		FVector Pos = GetActorLocation();
		Pos.X += EnterSpeed * DeltaTime;
		if (Pos.X >= HoverX)
		{
			Pos.X = HoverX;
			bHasEntered = true;
			HoverYOrigin = Pos.Y;
		}
		SetActorLocation(Pos, true);
		return;
	}

	// 悬停扫射：左右摆动（正弦）
	FVector Pos2 = GetActorLocation();
	Pos2.Y = HoverYOrigin + FMath::Sin(AliveTime * StrafeSpeed) * StrafeAmplitude;
	SetActorLocation(Pos2, true);
}

void ABossEnemy::OnBossHealthChanged(float CurrentHealth)
{
	CheckPhaseTransition();
}

void ABossEnemy::CheckPhaseTransition()
{
	if (Phases.Num() == 0) return;

	float Percent = HealthComponent ? HealthComponent->GetHealthPercent() : 0.0f;

	// 反向遍历：找到最大的「血量门限 <= 当前血量比例」的阶段
	// 满血→阶段0，降到门限以下→进入下一阶段
	for (int32 i = Phases.Num() - 1; i >= 0; i--)
	{
		if (Percent <= Phases[i].HealthThreshold)
		{
			if (CurrentPhaseIndex != i)
			{
				CurrentPhaseIndex = i;
				BossFireTimer = 0.0f; // 切换阶段立即开火
				UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Boss %s 进入阶段 %d (血量 %.0f%%)"),
					*GetName(), i, Percent * 100.0f);
			}
			break;
		}
	}
}

void ABossEnemy::FireBurst()
{
	if (!ProjectileClass || !GetWorld()) return;
	if (Phases.Num() == 0) return;

	const FEnemyPhase& Phase = Phases[FMath::Clamp(CurrentPhaseIndex, 0, Phases.Num() - 1)];
	FVector Origin = GetActorLocation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 发射单个弹幕的辅助 lambda
	auto SpawnOne = [&](FVector Dir)
	{
		AProjectileBase* Proj = GetWorld()->SpawnActor<AProjectileBase>(
			ProjectileClass, Origin, Dir.Rotation(), SpawnParams);
		if (Proj)
		{
			Proj->Initialize(Phase.BulletSpeed, Phase.BulletDamage, EProjectileFaction::Enemy);
		}
	};

	switch (Phase.PatternType)
	{
		case EBossPatternType::Aimed:
		{
			// 朝玩家方向瞄准
			APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
			FVector Dir = Player ? (Player->GetActorLocation() - Origin).GetSafeNormal()
			                     : FVector(-1.0f, 0.0f, 0.0f);
			SpawnOne(Dir);
			break;
		}

		case EBossPatternType::Spread:
		{
			// 朝下（-X）扇形散射
			float Half = Phase.SpreadAngle * 0.5f;
			float Step = Phase.BulletCount > 1 ? Phase.SpreadAngle / (Phase.BulletCount - 1) : 0.0f;
			float Start = -Half;
			FVector BaseDir = FVector(-1.0f, 0.0f, 0.0f);
			for (int32 i = 0; i < Phase.BulletCount; i++)
			{
				float Angle = FMath::DegreesToRadians(Start + Step * i);
				SpawnOne(BaseDir.RotateAngleAxisRad(Angle, FVector::UpVector));
			}
			break;
		}

		case EBossPatternType::Circle:
		{
			// 环形弹幕
			float Step = 360.0f / Phase.BulletCount;
			FVector BaseDir = FVector(-1.0f, 0.0f, 0.0f);
			for (int32 i = 0; i < Phase.BulletCount; i++)
			{
				float Angle = FMath::DegreesToRadians(Step * i);
				SpawnOne(BaseDir.RotateAngleAxisRad(Angle, FVector::UpVector));
			}
			break;
		}
	}
}
