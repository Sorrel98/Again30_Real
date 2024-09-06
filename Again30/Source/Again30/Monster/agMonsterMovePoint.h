// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "GameFramework/Actor.h"
#include "agMonsterMovePoint.generated.h"

UCLASS()
class AGAIN30_API AagMonsterMovePoint : public AActor
{
	GENERATED_BODY()

public:
	AagMonsterMovePoint();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite)
	EagMonsterMovePointType _pointType = EagMonsterMovePointType::None;
};
