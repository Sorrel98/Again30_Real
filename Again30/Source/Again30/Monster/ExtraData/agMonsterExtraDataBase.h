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
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster", meta=(ClampMin = 0))
	float MaxHp = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster", meta=(ClampMin = 0))
	float MaxTiredGage = 0.f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|Spawn")
	EagMonsterMovePointType InitSpawnMovePoint = EagMonsterMovePointType::None;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|Anim")
	FSoftObjectPath TakeDamageMontage;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|Anim")
	FSoftObjectPath CorpseMontage;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|Ui")
	float HpBarShowTime = 0.f;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|TiredDamage")
	float TickDamTime = 0.f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category="Again30|Monster|TiredDamage")
	float TickDam = 0.f;
};
