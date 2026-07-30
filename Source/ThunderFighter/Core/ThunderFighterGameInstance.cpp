// ThunderFighter - 雷霆战机 GameInstance 实现

#include "ThunderFighterGameInstance.h"

void UThunderFighterGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] GameInstance initialized"));
}

bool UThunderFighterGameInstance::TryUpdateHighScore(int32 NewScore)
{
	if (NewScore > HighScore)
	{
		HighScore = NewScore;
		return true;
	}
	return false;
}

void UThunderFighterGameInstance::ResetGameState()
{
	Lives = 3;
	BombCount = 3;
	WeaponLevel = 1;
	UE_LOG(LogTemp, Log, TEXT("[ThunderFighter] Game state reset"));
}
