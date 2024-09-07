// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "agManagerBase.h"
#include "Again30/Library/agEnumClass.h"
#include "agMonsterMoveManager.generated.h"

USTRUCT()
struct FvMonsterMoveData
{
	GENERATED_BODY()
	TObjectPtr<class AagMonsterBase> _movingMonster = nullptr;
	float _movingTotalTime = 0.f;
	float _movingElapsedTime = 0.f;
	FVector _movingStartLocation = FVector::ZeroVector;
	FVector _movingEndLocation = FVector::ZeroVector;
};

/**
 *  아 이거 manager 로 관리하는 게 아니라 monsterbase에 붙였어야 했나
 */
UCLASS()
class AGAIN30_API UagMonsterMoveManager : public UagManagerBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float elapsedTime) override;
	void AddMonsterMovePoint( EagMonsterMovePointType type, const TObjectPtr<class AagMonsterMovePoint>& movePoint );
	bool GetMovePointLocation( EagMonsterMovePointType type, FVector& location );

	void RequestMoveToPoint(const TObjectPtr<class AagMonsterBase>& monster, EagMonsterMovePointType pointType, float duration);

protected:
	void _move(float elapsedTime);
	
	UPROPERTY()
	TMap<EagMonsterMovePointType, TObjectPtr<class AagMonsterMovePoint>> _pointContainer;

	UPROPERTY()
	TMap<int32, FvMonsterMoveData> _movingContainer;
};
