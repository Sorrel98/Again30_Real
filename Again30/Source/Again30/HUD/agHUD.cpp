#include "agHUD.h"
#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/GameMode/agGameModeExtraData.h"
#include "Components/SlateWrapperTypes.h"
#include "MainPlay/agMainPlayWidget.h"

void AagHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MainPlayGameWidget = CreateWidgetInHud(MainPlayWidgetClass, ESlateVisibility::Visible);
}

void AagHUD::BeginPlay()
{
	Super::BeginPlay();

	BindWidgetDelegates();
}

void AagHUD::SetGameTimeText(float CurrentGameTime)
{
	if(MainPlayGameWidget)
	{
		MainPlayGameWidget->SetGameTimeText(CurrentGameTime);
	}
}

void AagHUD::SetGenerationText(int32 Generation)
{
	if(MainPlayGameWidget)
    {
    	MainPlayGameWidget->SetGenerationText(Generation);
    }
}

void AagHUD::SetWinText()
{
	if(MainPlayGameWidget)
	{
		MainPlayGameWidget->SetWinText();
	}
}

void AagHUD::BindWidgetDelegates()
{
	if(AagPlayGameMode* GameMode = GetWorld()->GetAuthGameMode<AagPlayGameMode>())
	{
		GameMode->OnCurrentTimeChanged.AddUObject(this, &AagHUD::SetGameTimeText);
		GameMode->OnGenerationChanged.AddUObject(this, &AagHUD::SetGenerationText);
		GameMode->OnGameEndEvent.AddUObject(this, &AagHUD::SetWinText);

		// generation time 초기값 셋팅
		if( GameMode->GetExtraData() ){
			SetGameTimeText( GameMode->GetExtraData()->GenerationTime );
		}
	}
}
