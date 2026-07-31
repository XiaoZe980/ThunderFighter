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
		FireTimer = 1.0f / BaseFireRate;
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

	if (!Pattern || Pattern->Num() == 0) return;

	for (const FVector& Offset : *Pattern)
	{
		FireProjectile(Offset);
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
		Projectile->Initialize(ProjectileSpeed, ProjectileDamage, EProjectileFaction::Player);
		UE_LOG(LogTemp, Verbose, TEXT("[ThunderFighter] 生成子弹: 位置=(%.0f, %.0f, %.0f) 朝向=%s"),
			SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z, *SpawnRotation.ToString());
	}
}
