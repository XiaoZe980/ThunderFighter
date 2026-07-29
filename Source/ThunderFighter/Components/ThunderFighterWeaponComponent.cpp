// ThunderFighter - 雷霆战机 WeaponComponent Implementation

#include "ThunderFighterWeaponComponent.h"
#include "Actors/ProjectileBase.h"
#include "Engine/World.h"

UThunderFighterWeaponComponent::UThunderFighterWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default firing patterns at various weapon levels
	// Level 1: single center shot
	PatternLevel1.Add(FVector(100.0f, 0.0f, 0.0f));

	// Level 2: twin shot
	PatternLevel2.Add(FVector(100.0f, -20.0f, 0.0f));
	PatternLevel2.Add(FVector(100.0f, 20.0f, 0.0f));

	// Level 3: triple shot
	PatternLevel3.Add(FVector(100.0f, -30.0f, 0.0f));
	PatternLevel3.Add(FVector(100.0f, 0.0f, 0.0f));
	PatternLevel3.Add(FVector(100.0f, 30.0f, 0.0f));

	// Level 4: quad shot
	PatternLevel4.Add(FVector(100.0f, -40.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, -15.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, 15.0f, 0.0f));
	PatternLevel4.Add(FVector(100.0f, 40.0f, 0.0f));

	// Level 5: wide spread
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
	FireTimer = 0.0f; // Fire immediately
}

void UThunderFighterWeaponComponent::StopFiring()
{
	bIsFiring = false;
}

void UThunderFighterWeaponComponent::ActivateBomb()
{
	if (BombCount <= 0) return;

	BombCount--;

	// Bomb clears all enemy projectiles and damages all enemies on screen
	// Implementation will connect to a manager or iterate over actors
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
	if (!ProjectileClass) return;

	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetActorRotation().RotateVector(LocalOffset);
	FRotator SpawnRotation = Owner->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(
		ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (Projectile)
	{
		Projectile->Initialize(ProjectileSpeed, ProjectileDamage, EProjectileFaction::Player);
	}
}
