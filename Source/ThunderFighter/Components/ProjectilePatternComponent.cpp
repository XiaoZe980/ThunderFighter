// ThunderFighter - 雷霆战机 ProjectilePatternComponent 实现

#include "ProjectilePatternComponent.h"
#include "Actors/ProjectileBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UProjectilePatternComponent::UProjectilePatternComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectilePatternComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bAutoFire) return;

	AutoFireTimer -= DeltaTime;
	if (AutoFireTimer <= 0.0f)
	{
		AutoFireTimer = AutoFireInterval;
		// 自动射击朝玩家方向发射
		FireAtPlayer(AutoFireSpeed, AutoFireDamage);
	}
}

void UProjectilePatternComponent::FireSingle(FVector Direction, float Speed, float Damage)
{
	SpawnProjectile(Direction.GetSafeNormal(), Speed, Damage);
}

void UProjectilePatternComponent::FireSpread(int32 Count, float SpreadAngle, float Speed, float Damage)
{
	if (Count <= 0) return;

	float HalfSpread = SpreadAngle * 0.5f;
	float AngleStep = (Count > 1) ? (SpreadAngle / (Count - 1)) : 0.0f;
	float StartAngle = -HalfSpread;

	FVector BaseDirection = FVector(1.0f, 0.0f, 0.0f); // 游戏空间中的正前方

	for (int32 i = 0; i < Count; i++)
	{
		float Angle = FMath::DegreesToRadians(StartAngle + AngleStep * i);
		FVector Dir = BaseDirection.RotateAngleAxisRad(Angle, FVector::UpVector);
		SpawnProjectile(Dir, Speed, Damage);
	}
}

void UProjectilePatternComponent::FireCircle(int32 Count, float Speed, float Damage, float StartAngle)
{
	if (Count <= 0) return;

	float AngleStep = 360.0f / Count;

	for (int32 i = 0; i < Count; i++)
	{
		float Angle = FMath::DegreesToRadians(StartAngle + AngleStep * i);
		FVector Dir = FVector(1.0f, 0.0f, 0.0f).RotateAngleAxisRad(Angle, FVector::UpVector);
		SpawnProjectile(Dir, Speed, Damage);
	}
}

void UProjectilePatternComponent::FireAtTarget(FVector TargetLocation, float Speed, float Damage, float Inaccuracy)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Direction = (TargetLocation - Owner->GetActorLocation()).GetSafeNormal();

	if (Inaccuracy > 0.0f)
	{
		float RandomAngle = FMath::DegreesToRadians(FMath::FRandRange(-Inaccuracy, Inaccuracy));
		Direction = Direction.RotateAngleAxisRad(RandomAngle, FVector::UpVector);
	}

	SpawnProjectile(Direction, Speed, Damage);
}

void UProjectilePatternComponent::FireAtPlayer(float Speed, float Damage, float Inaccuracy)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		FireAtTarget(PlayerPawn->GetActorLocation(), Speed, Damage, Inaccuracy);
	}
}

void UProjectilePatternComponent::SetAutoFire(bool bEnable, float FireInterval)
{
	bAutoFire = bEnable;
	AutoFireInterval = FireInterval;
	AutoFireTimer = 0.0f;
}

AProjectileBase* UProjectilePatternComponent::SpawnProjectile(FVector Direction, float Speed, float Damage)
{
	if (!ProjectileClass || !GetWorld()) return nullptr;

	AActor* Owner = GetOwner();
	if (!Owner) return nullptr;

	FVector SpawnLocation = Owner->GetActorLocation();
	FRotator SpawnRotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Initialize(Speed, Damage, Faction);
	}

	return Projectile;
}
