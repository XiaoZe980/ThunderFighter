// ThunderFighter - 雷霆战机 MainMenuWidget 实现

#include "MainMenuWidget.h"
#include "Core/ThunderFighterGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClicked);
	}
	if (ShopButton)
	{
		ShopButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnShopClicked);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}

	// 显示金币
	if (GoldText)
	{
		if (UThunderFighterGameInstance* GI = Cast<UThunderFighterGameInstance>(GetGameInstance()))
		{
			GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->GetGold())));
		}
	}
}

void UMainMenuWidget::OnStartClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("L_Stage_01"));
}

void UMainMenuWidget::OnShopClicked()
{
	UGameplayStatics::OpenLevel(this, TEXT("L_UpgradeShop"));
}

void UMainMenuWidget::OnQuitClicked()
{
	APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (PC)
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
	}
}
