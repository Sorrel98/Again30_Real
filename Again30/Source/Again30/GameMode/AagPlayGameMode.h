﻿#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "GameFramework/GameMode.h"
#include "AagPlayGameMode.generated.h"

class AagFish;
class APlayerStart;
class AagPlayGameMode;

DECLARE_EVENT(AagPlayGameMode, FOnGameModeEvent);
DECLARE_EVENT_OneParam(AagPlayGameMode, FOnGenerationChanged, int32);
DECLARE_EVENT_OneParam(AagPlayGameMode, FOnTimeChanged, float);

UCLASS()
class AGAIN30_API AagPlayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	FOnGameModeEvent OnGameEndEvent;
	FOnGenerationChanged OnGenerationChanged;	
	FOnTimeChanged OnCurrentTimeChanged;
	
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

	/** 현재 물고기 사망-스폰 연출 중인지 여부 */
	bool bNowDoingFishProduction;

	// Caching
	UPROPERTY()
	TObjectPtr<AagFish> CurrentFish;
	UPROPERTY()
	TObjectPtr<APlayerController> CurrentPlayController;

public:
	AagPlayGameMode();
	virtual void PostInitializeComponents() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void GameStart();
	void GenerationStart();
	void GenerationEnd();
	void IncreaseGeneration();
	void FishWin();

	// Manager
	bool GetManager(EagManagerType type, TObjectPtr<class UagManagerBase>& manager);
	template <typename MangerClassName>
	bool GetManager(EagManagerType type, TObjectPtr<MangerClassName>& manager)
	{
		TObjectPtr<UagManagerBase> managerbase;
		if( GetManager(type, managerbase) == false)
		{
			return false;
		}
		if( managerbase != nullptr )
		{
			manager = Cast<MangerClassName>(managerbase);
			return true;
		}
		return false;
	}
	void PostCreateManager();

	// Monster
	int32 GetNewMonsterUID();
	void RegisterMovePoint(EagMonsterMovePointType type, TObjectPtr<class AagMonsterMovePoint> movePoint);
	void AddMonsterMovePoint(EagMonsterMovePointType type, const TObjectPtr<AagMonsterMovePoint>& movePoint);
	bool GetMovePointLocation(EagMonsterMovePointType type, FVector& location);

private:
	void CalculateGenerationTime(float DeltaSeconds);
	void SpawnFish();
	APlayerStart* GetPlayerStartPoint();

	/** 물고기 사망 연출이 끝난 뒤 불리게 될 함수 */
	void OnFishDeadProductionEnd();
	/** 물고기 스폰 연출이 끝난 뒤 불리게 될 함수 */
	void OnFishSpawnProductionEnd();
	
protected:
	// manager
	void _setManagerContainer();
	TObjectPtr<class UagManagerBase> _createManager(EagManagerType type);
	void _managerTick(float elapsedTime);
	
	UPROPERTY()
	TObjectPtr<class UagGameModeExtraData> _extraData = nullptr;

	UPROPERTY()
	TMap<EagManagerType, TObjectPtr<class UagManagerBase>> _managerContainer;
	// monster
	UPROPERTY()
	TObjectPtr<class AagMonsterBase> _monster = nullptr;
	UPROPERTY()
	TMap<EagMonsterMovePointType, TObjectPtr<class AagMonsterMovePoint>> _pointContainer;
	int32 _monsterUID = 1;
};
