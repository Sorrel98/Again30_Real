#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "agHUD.generated.h"


class UagMainPlayWidget;

UCLASS()
class AGAIN30_API AagHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainPlayWidget", meta=(AllowPrivateAccess=true))
	TSubclassOf<UagMainPlayWidget> MainPlayWidgetClass;
	TObjectPtr<UagMainPlayWidget> MainPlayGameWidget;

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	/** 망할 인터페이스는 없다. 자세한 주석도 없다. 그냥 MainPlayGameWidget 전달용 함수들 */
	void SetGameTimeText(float CurrentGameTime);
	void SetGenerationText(int32 Generation);
	void SetWinText();
	
	void BindWidgetDelegates();
	template <typename WidgetT = UUserWidget>
	WidgetT* CreateWidgetInHud(TSubclassOf<WidgetT> WidgetClass, ESlateVisibility Visibility);
};

template <typename WidgetT>
WidgetT* AagHUD::CreateWidgetInHud(TSubclassOf<WidgetT> WidgetClass, ESlateVisibility Visibility)
{
	WidgetT* CreatedWidget = CreateWidget<WidgetT>(PlayerOwner, WidgetClass);
	if(CreatedWidget)
	{
		CreatedWidget->AddToViewport();
		CreatedWidget->SetVisibility(Visibility);
	}
	return CreatedWidget;
}
