#include "agMainPlayWidget.h"

#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void UagMainPlayWidget::SetGameTimeText(float CurrentGameTime)
{
	if(TimeText == nullptr) [[unlikely]]
	{
		return;
	}
	
	int32 Seconds = FMath::Floor(CurrentGameTime);
	int32 Milliseconds = FMath::Floor((CurrentGameTime - Seconds)*100.f);
	FString GameTimeString = FString::Printf(TEXT("%d.%d"), Seconds, Milliseconds);
	TimeText->SetText(FText::FromString(GameTimeString));
}

void UagMainPlayWidget::SetGenerationText(int32 Generation)
{
	if(GenerationText == nullptr) [[unlikely]]
	{
		return;
	}

	FString GenerationString = FString::Printf(TEXT("%d 세대의 물고기"), Generation);
	GenerationText->SetText(FText::FromString(GenerationString));
}

void UagMainPlayWidget::SetWinText()
{
	if(GameEndOverlay)
	{
		GameEndOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}
