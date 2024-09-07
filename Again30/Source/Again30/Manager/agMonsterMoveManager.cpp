// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterMoveManager.h"

#include "Again30/Monster/agMonsterBase.h"
#include "Again30/Monster/agMonsterMovePoint.h"

void UagMonsterMoveManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UagMonsterMoveManager::Tick(float elapsedTime)
{
	Super::Tick(elapsedTime);

	if( _movingContainer.IsEmpty() == false ){
		_move(elapsedTime);
	}
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
	if( _pointContainer.Contains(type) == false ){
		return false;
	}
	location = _pointContainer[type]->GetActorLocation();
	return true;
}

void UagMonsterMoveManager::RequestMoveToPoint(const TObjectPtr<class AagMonsterBase>& monster, EagMonsterMovePointType pointType, float duration)
{
	if( monster == nullptr ){
		return;
	}
	FvMonsterMoveData newData = FvMonsterMoveData();
	if( GetMovePointLocation( pointType, newData._movingEndLocation ) == false ){
		return;
	}
	newData._movingStartLocation = monster->GetActorLocation();
	newData._movingMonster = monster;
	newData._movingTotalTime = duration;
	_movingContainer.Add( monster->GetUID(), newData);
}

void UagMonsterMoveManager::_move(float elapsedTime)
{
	for( auto iterator = _movingContainer.CreateIterator(); iterator; ++iterator ){
		auto& movingData = iterator.Value();
		if( movingData._movingElapsedTime >= movingData._movingTotalTime ){
			iterator.RemoveCurrent();
		}
		if( movingData._movingMonster == nullptr ){
			continue;
		}
		movingData._movingMonster->SetActorLocation(FMath::Lerp(movingData._movingStartLocation,
																movingData._movingEndLocation,
																FMath::Clamp(
																	movingData._movingElapsedTime / movingData.
																	_movingTotalTime, 0, 1)));
		movingData._movingElapsedTime += elapsedTime;
	}
}
