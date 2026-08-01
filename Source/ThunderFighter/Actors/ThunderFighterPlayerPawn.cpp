// ThunderFighter - 雷霆战机 PlayerPawn 实现

#include "ThunderFighterPlayerPawn.h"
#include "EnemyBase.h"
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

	// 根碰撞盒
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionBox->SetGenerateOverlapEvents(true);
	SetRootComponent(CollisionBox);

	// 玩家 Tag，用于敌方子弹识别阵营
	Tags.Add(TEXT("Player"));

	// 战机网格体
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 生命值组件
	HealthComponent = CreateDefaultSubobject<UThunderFighterHealthComponent>(TEXT("HealthComponent"));

	// 武器组件
	WeaponComponent = CreateDefaultSubobject<UThunderFighterWeaponComponent>(TEXT("WeaponComponent"));

	// 弹簧臂——将摄像机悬停在玩家上方，俯视视角
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 3000.0f;                       // 战场上方高度
	CameraSpringArm->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f)); // 向下倾斜以实现俯视视角
	CameraSpringArm->bDoCollisionTest = false;                        // 不与几何体碰撞
	CameraSpringArm->bInheritPitch = false;
	CameraSpringArm->bInheritYaw = false;
	CameraSpringArm->bInheritRoll = false;

	// 俯视摄像机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	FollowCamera->SetFieldOfView(60.0f);
}

void AThunderFighterPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// 绑定生命值耗尽事件
	if (HealthComponent)
	{
		HealthComponent->OnHealthDepleted.AddDynamic(this, &AThunderFighterPlayerPawn::OnHealthDepleted);
	}

	// 绑定重叠事件：敌人或敌方子弹碰到玩家时造成伤害
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AThunderFighterPlayerPawn::OnOverlap);

	// 自动射击：玩家自动开火，无需按住按钮
	if (bAutoFireEnabled)
	{
		StartFiring();
	}
}

void AThunderFighterPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 处理无敌计时器
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

void AThunderFighterPlayerPawn::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 无敌状态下不受伤
	if (bIsInvincible) return;
	if (!OtherActor || OtherActor == this) return;

	// 检查是否是敌方阵营（敌人或敌方子弹）
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		if (HealthComponent)
		{
			// 从敌人身上读取碰撞伤害（不同敌人不同伤害值）
			float Damage = CollisionDamage; // 默认值
			if (AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor))
			{
				Damage = Enemy->CollisionDamage;
			}

			HealthComponent->TakeDamage(Damage);
			UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player collided with %s (%.0f damage)"), *OtherActor->GetName(), Damage);
		}

		// 敌人碰撞玩家后自毁（和子弹命中一样）
		OtherActor->Destroy();

		// 受伤后短暂无敌
		GrantInvincibility(1.0f);
	}
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

	// 将边距作为视口边缘的比例应用
	const float MarginX = ViewportX * ScreenBoundaryMargin;
	const float MarginY = ViewportY * ScreenBoundaryMargin;

	// 反投影屏幕的 4 个角（带边距），找出它们与玩家 Z 平面的交点
	auto DeprojectToPlane = [&](float ScreenX, float ScreenY) -> FVector2D
	{
		FVector Origin, Direction;
		PC->DeprojectScreenPositionToWorld(ScreenX, ScreenY, Origin, Direction);

		// 计算射线与玩家 Z 坐标处水平面的交点
		float PlayerZ = GetActorLocation().Z;
		float T = (PlayerZ - Origin.Z) / Direction.Z;
		FVector HitPoint = Origin + Direction * T;
		return FVector2D(HitPoint.X, HitPoint.Y);
	};

	FVector2D TopLeft     = DeprojectToPlane(MarginX, MarginY);
	FVector2D TopRight    = DeprojectToPlane(ViewportX - MarginX, MarginY);
	FVector2D BottomLeft  = DeprojectToPlane(MarginX, ViewportY - MarginY);
	FVector2D BottomRight = DeprojectToPlane(ViewportX - MarginX, ViewportY - MarginY);

	OutMinX = FMath::Min(FMath::Min(TopLeft.X, TopRight.X), FMath::Min(BottomLeft.X, BottomRight.X));
	OutMaxX = FMath::Max(FMath::Max(TopLeft.X, TopRight.X), FMath::Max(BottomLeft.X, BottomRight.X));
	OutMinY = FMath::Min(FMath::Min(TopLeft.Y, TopRight.Y), FMath::Min(BottomLeft.Y, BottomRight.Y));
	OutMaxY = FMath::Max(FMath::Max(TopLeft.Y, TopRight.Y), FMath::Max(BottomLeft.Y, BottomRight.Y));
}

// 生命值耗尽回调——在 BeginPlay 中绑定
void AThunderFighterPlayerPawn::OnHealthDepleted()
{
	StopFiring();

	// 禁用碰撞
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 通知 GameMode
	if (AThunderFighterGameMode* GM = Cast<AThunderFighterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnPlayerDefeated();
	}

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Player ship destroyed!"));
}
