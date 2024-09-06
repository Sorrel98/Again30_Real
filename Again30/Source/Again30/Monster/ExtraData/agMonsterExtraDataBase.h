// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "Engine/DataAsset.h"
#include "agMonsterExtraDataBase.generated.h"

/**
 * 
 */
UCLASS()
class AGAIN30_API UagMonsterExtraDataBase : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|Spawn")
	EagMonsterMovePointType InitSpawnMovePoint = EagMonsterMovePointType::None;
};
