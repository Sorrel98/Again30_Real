#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "agMainPlayWidget.generated.h"


class UOverlay;
class UTextBlock;

UCLASS()
class AGAIN30_API UagMainPlayWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> GenerationText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimeText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> GameEndOverlay;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void GameEnd();
	void SetGameTimeText(float CurrentGameTime);
	void SetGenerationText(int32 Generation);
	void SetWinText();
};
