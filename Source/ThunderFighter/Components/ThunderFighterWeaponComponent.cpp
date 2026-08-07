// ThunderFighter - 雷霆战机 WeaponComponent 实现

#include "ThunderFighterWeaponComponent.h"
#include "Actors/ProjectileBase.h"
#include "Engine/World.h"

UThunderFighterWeaponComponent::UThunderFighterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 各武器等级的默认射击模式
	// 生成位置 = 玩家位置 + 前方100单位，发射方向 = 玩家朝向
	PatternLevel1.Add(FVector(100.0f, 0.0f, 0.0f));

	// 2 级：双发
	PatternLevel2.Add(FVector(100.0f, -20.0f, 0.0f));
	PatternLevel2.Add(FVector(100.0f, 20.0f, 0.0f));

	// 3 级：三连发
	PatternLevel3.Add(FVector(100.0f, -30.0f, 0.0f));
	PatternLevel3.Add(FVector(100.0f, 0.0f, 0.0f));
	PatternLevel3.Add(FVector(100.0f, 30.0f, 0.0f));

	// 4 级：四连发
	PatternLevel4.Add(FVector(100.0f, -40.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, -15.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, 15.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, 40.0f, 0.0f));

	// 5 级：宽幅扩散
	PatternLevel5.Add(FVector(100.0f, -50.0f, 0.0f));
	PatternLevel5.Add(FVector(100.0f, -25.0f, 0.0f));
	PatternLevel5.Add(FVector(100.0f, 0.0f, 0.0f));
	PatternLevel5.Add(FVector(100.0f, 25.0f, 0.0f));
	PatternLevel5.Add(FVector(100.0f, 50.0f, 0.0f));
}

void UThunderFighterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UThunderFighterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsFiring) return;

	FireTimer -= DeltaTime;
	if (FireTimer <= 0.0f)
	{
		FirePattern();
		// 射速受强化倍率影响
		float FinalFireRate = BaseFireRate * FMath::Max(0.01f, Modifiers.FireRateMultiplier);
		FireTimer = 1.0f / FinalFireRate;
	}
}

void UThunderFighterWeaponComponent::StartFiring()
{
	bIsFiring = true;
	FireTimer = 0.0f; // 立即开火
}

void UThunderFighterWeaponComponent::StopFiring()
{
	bIsFiring = false;
}

void UThunderFighterWeaponComponent::ActivateBomb()
{
	if (BombCount <= 0) return;

	BombCount--;

	// 炸弹清除所有敌方弹幕并对屏幕上所有敌人造成伤害
	// 实现将与管理器连接或遍历所有 Actor
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] BOMB activated! %d remaining"), BombCount);
}

void UThunderFighterWeaponComponent::AddBomb(int32 Count)
{
	BombCount += Count;
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Bomb added! Total: %d"), BombCount);
}

void UThunderFighterWeaponComponent::UpgradeWeapon()
{
	WeaponLevel = FMath::Min(WeaponLevel + 1, MaxWeaponLevel);
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Weapon upgraded to level %d"), WeaponLevel);
}

void UThunderFighterWeaponComponent::FirePattern()
{
	if (!GetWorld()) return;

	// 1. 基础武器等级模式
	const TArray<FVector>* Pattern = nullptr;
	switch (WeaponLevel)
	{
		case 1: Pattern = &PatternLevel1; break;
		case 2: Pattern = &PatternLevel2; break;
		case 3: Pattern = &PatternLevel3; break;
		case 4: Pattern = &PatternLevel4; break;
		case 5: Pattern = &PatternLevel5; break;
		default: Pattern = &PatternLevel1; break;
	}

	if (Pattern)
	{
		for (const FVector& Offset : *Pattern)
		{
			FireProjectile(Offset);
		}
	}

	// 2. 强化额外弹道：在基础两侧对称叠加
	for (int32 i = 1; i <= Modifiers.BonusBullets; i++)
	{
		float Spread = 25.0f * i;
		FireProjectile(FVector(100.0f, -Spread, 0.0f));
		FireProjectile(FVector(100.0f, Spread, 0.0f));
	}

	// 3. 侧翼僚机：在更远的两侧发射
	const float WingOffset = 70.0f;
	for (int32 i = 1; i <= Modifiers.SideWingCount; i++)
	{
		FireProjectile(FVector(100.0f, -WingOffset * i, 0.0f));
		FireProjectile(FVector(100.0f, WingOffset * i, 0.0f));
	}
}

void UThunderFighterWeaponComponent::FireProjectile(FVector LocalOffset)
{
	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ThunderFighter] 武器组件 ProjectileClass 未设置，无法生成子弹！"));
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner) return;

	// 生成位置 = 玩家位置 + 偏移，子弹朝向 = 固定朝屏幕上方（+X 轴），不随玩家转向
	FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorRotation().RotateVector(LocalOffset);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f); // 固定朝 +X 方向（屏幕上方）

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		// 伤害受强化倍率影响
		float FinalDamage = ProjectileDamage * FMath::Max(0.01f, Modifiers.DamageMultiplier);

		Projectile->Initialize(ProjectileSpeed, FinalDamage, EProjectileFaction::Player);
		Projectile->SetPiercing(Modifiers.bPiercing);
		Projectile->SetHoming(Modifiers.bHoming);
		Projectile->SetBounceCount(Modifiers.BounceCount);
	}
}

void UThunderFighterWeaponComponent::ApplyModifier(EUpgradeEffect Effect, float Value)
{
	switch (Effect)
	{
		case EUpgradeEffect::BonusBullets:
			Modifiers.BonusBullets += (int32)Value;
			break;
		case EUpgradeEffect::DamageMultiplier:
			Modifiers.DamageMultiplier += Value;
			break;
		case EUpgradeEffect::FireRateMultiplier:
			Modifiers.FireRateMultiplier += Value;
			break;
		case EUpgradeEffect::Piercing:
			Modifiers.bPiercing = true;
			break;
		case EUpgradeEffect::Homing:
			Modifiers.bHoming = true;
			break;
		case EUpgradeEffect::Bounce:
			Modifiers.BounceCount += (int32)Value;
			break;
		case EUpgradeEffect::SideWings:
			Modifiers.SideWingCount += (int32)Value;
			break;
		case EUpgradeEffect::BombCount:
			AddBomb((int32)Value);
			break;
		default:
			break;
	}
}
