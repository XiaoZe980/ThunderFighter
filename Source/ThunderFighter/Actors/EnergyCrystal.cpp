// ThunderFighter - 雷霆战机 EnergyCrystal 实现

#include "EnergyCrystal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PlayerLevelComponent.h"
#include "ThunderFighterPlayerPawn.h"
#include "Kismet/GameplayStatics.h"

AEnergyCrystal::AEnergyCrystal()
{
	PrimaryActorTick.bCanEverTick = true;

	// 拾取触发球体
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetSphereRadius(40.0f);
	TriggerSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerSphere->SetGenerateOverlapEvents(true);
	SetRootComponent(TriggerSphere);

	// 视觉网格体
	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	CrystalMesh->SetupAttachment(RootComponent);
	CrystalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnergyCrystal::BeginPlay()
{
	Super::BeginPlay();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnergyCrystal::OnOverlap);
	LifetimeTimer = 0.0f;
}

void AEnergyCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifetimeTimer += DeltaTime;
	if (LifetimeTimer >= Lifetime)
	{
		Destroy();
		return;
	}

	// 查找玩家
	AThunderFighterPlayerPawn* Player = Cast<AThunderFighterPlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (Player)
	{
		FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
		float Distance = ToPlayer.Size();

		if (Distance <= MagnetRadius)
		{
			bIsMagnetizing = true;
		}

		if (bIsMagnetizing)
		{
			// 加速飞向玩家
			FVector Dir = ToPlayer.GetSafeNormal();
			AddActorWorldOffset(Dir * MagnetSpeed * DeltaTime, true);
		}
	}
}

void AEnergyCrystal::Initialize(float InExpAmount)
{
	ExpAmount = InExpAmount;
}

void AEnergyCrystal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	// 玩家触碰经验球
	if (OtherActor->IsA<AThunderFighterPlayerPawn>())
	{
		if (UPlayerLevelComponent* LevelComp = OtherActor->FindComponentByClass<UPlayerLevelComponent>())
		{
			LevelComp->AddExperience(ExpAmount);
		}

		Destroy();
	}
}
