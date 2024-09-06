// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterMoveManager.h"

#include "Again30/Monster/agMonsterMovePoint.h"

void UagMonsterMoveManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UagMonsterMoveManager::AddMonsterMovePoint(EagMonsterMovePointType type, const TObjectPtr<AagMonsterMovePoint>& movePoint)
{
	if( movePoint == nullptr )
	{
		return;
	}
	if( _pointContainer.Contains(type) == true )
	{
		return;
	}
	_pointContainer.Add(type, movePoint);
}

bool UagMonsterMoveManager::GetMovePointLocation(EagMonsterMovePointType type, FVector& location)
{
	if( _pointContainer.Contains(type) == false )
	{
		return false;
	}
	location = _pointContainer[type]->GetActorLocation();
	return true;
}
