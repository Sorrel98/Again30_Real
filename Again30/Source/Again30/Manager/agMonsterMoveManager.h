// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "agManagerBase.h"
#include "Again30/Library/agEnumClass.h"
#include "agMonsterMoveManager.generated.h"

/**
 * 
 */
UCLASS()
class AGAIN30_API UagMonsterMoveManager : public UagManagerBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void AddMonsterMovePoint( EagMonsterMovePointType type, const TObjectPtr<class AagMonsterMovePoint>& movePoint );
	bool GetMovePointLocation( EagMonsterMovePointType type, FVector& location );

protected:
	UPROPERTY()
	TMap<EagMonsterMovePointType, TObjectPtr<class AagMonsterMovePoint>> _pointContainer;
};
