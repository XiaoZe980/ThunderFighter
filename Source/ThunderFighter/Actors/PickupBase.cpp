// ThunderFighter - 雷霆战机 PickupBase 实现

#include "PickupBase.h"
#include "Actors/ThunderFighterPlayerPawn.h"
#include "Components/ThunderFighterWeaponComponent.h"
#include "Components/ThunderFighterHealthComponent.h"
#include "Core/ThunderFighterGameMode.h"
#include "Core/ThunderFighterGameInstance.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 触发球体
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetSphereRadius(40.0f);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(TriggerSphere);

	// 视觉网格体
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlap);

	InitialZ = GetActorLocation().Z;
	LifetimeTimer = 0.0f;
}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 浮动动画（上下抖动）
	LifetimeTimer += DeltaTime;
	FVector Pos = GetActorLocation();
	Pos.Z = InitialZ + FMath::Sin(LifetimeTimer * FloatSpeed) * FloatAmplitude;

	// 向下漂移（在游戏坐标系中为 X 方向）
	Pos.X -= DriftSpeed * DeltaTime;

	SetActorLocation(Pos);

	// 生命周期结束后自动销毁
	if (LifetimeTimer >= Lifetime)
	{
		Destroy();
	}
}

void APickupBase::Initialize(EPickupType InType, float InValue)
{
	PickupType = InType;
	Value = InValue;
}

void APickupBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	// 只有玩家可以收集拾取道具
	if (OtherActor->ActorHasTag(TEXT("Player")) || OtherActor->IsA<AThunderFighterPlayerPawn>())
	{
		ApplyEffect(OtherActor);
		Destroy();
	}
}

void APickupBase::ApplyEffect(AActor* PlayerActor)
{
	switch (PickupType)
	{
		case EPickupType::WeaponUpgrade:
		{
			UThunderFighterWeaponComponent* WeaponComp = PlayerActor->FindComponentByClass<UThunderFighterWeaponComponent>();
			if (WeaponComp)
			{
				WeaponComp->UpgradeWeapon();
				UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Weapon upgraded to level %d"), WeaponComp->GetWeaponLevel());
			}
			break;
		}

		case EPickupType::HealthRestore:
		{
			UThunderFighterHealthComponent* HealthComp = PlayerActor->FindComponentByClass<UThunderFighterHealthComponent>();
			if (HealthComp)
			{
				HealthComp->Heal(Value > 0.0f ? Value : 25.0f);
				UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Health restored by %.1f"), Value > 0.0f ? Value : 25.0f);
			}
			break;
		}

		case EPickupType::Bomb:
		{
			UThunderFighterWeaponComponent* WeaponComp = PlayerActor->FindComponentByClass<UThunderFighterWeaponComponent>();
			if (WeaponComp)
			{
				WeaponComp->AddBomb(1);
			}
			break;
		}

		case EPickupType::ScoreBonus:
		{
			int32 BonusPoints = FMath::RoundToInt(Value > 0.0f ? Value : 500.0f);
			AThunderFighterGameMode* GM = Cast<AThunderFighterGameMode>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->AddScore(BonusPoints);
			}
			UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Score bonus: +%d"), BonusPoints);
			break;
		}
	}
}
