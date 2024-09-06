#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "GameFramework/GameMode.h"
#include "AagPlayGameMode.generated.h"

class APlayerStart;
class AagPlayGameMode;

DECLARE_EVENT_OneParam(AagPlayGameMode, FOnValueChanged, int32);

UCLASS()
class AGAIN30_API AagPlayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	FOnValueChanged OnGenerationChanged;	
	FOnValueChanged OnCurrentTimeChanged;
	
private:
	/** 각 세대가 살 수 있는 시간 (30초) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rule" ,meta = (AllowPrivateAccess = true))
	float GenerationTime;
	/** 현재 세대가 지난 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rule" ,meta = (AllowPrivateAccess = true))
	float CurGenerationTime;
	/** 현재 몇 세대인지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rule" ,meta = (AllowPrivateAccess = true))
	int32 CurGeneration;

	// Caching

	UPROPERTY()
	TObjectPtr<APawn> CurrentFish;
	UPROPERTY()
	TObjectPtr<APlayerController> CurrentPlayController;

public:
	AagPlayGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void GameStart();
	void GenerationStart();

	void GenerationEnd();

	void IncreaseGeneration();
	
private:
	void SpawnFish();
	APlayerStart* GetPlayerStartPoint();


protected:
	TObjectPtr<class UagManagerBase> _createManager(EagManagerType type);
	
	UPROPERTY()
	TObjectPtr<class UagGameModeExtraData> _extraData = nullptr;

	UPROPERTY()
	TMap<EagManagerType, TObjectPtr<class UagManagerBase>> _managerContainer;
};
