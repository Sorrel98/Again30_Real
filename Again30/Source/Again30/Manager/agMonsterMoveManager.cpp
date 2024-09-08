// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterMoveManager.h"
#include "Again30/Monster/agMonsterBase.h"
#include "Kismet/KismetMathLibrary.h"

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

void UagMonsterMoveManager::PostCreated(TObjectPtr<class AagPlayGameMode> mode)
{
	Super::PostCreated(mode);
	_playGameMode = mode;
}

bool UagMonsterMoveManager::GetMovePointLocation(EagMonsterMovePointType type, FVector& location)
{
	if( _playGameMode == nullptr ){
		return false;
	}
	_playGameMode->GetMovePointLocation(type, location);
	return true;
}

bool UagMonsterMoveManager::GetMovePointRotation(EagMonsterMovePointType type, FRotator& rotator)
{
	if( _playGameMode == nullptr ){
		return false;
	}
	_playGameMode->GetMovePointRotation(type, rotator);
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
	newData._type = pointType;
	monster->SetState( EegMonsterState::Walking );

	if( monster->GetPrevPointType() != pointType ){
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(monster->GetActorLocation(), newData._movingEndLocation);
		monster->SetActorRotation(LookAtRotation);
	}
	monster->SetPrevPointType( pointType );
	
	_movingContainer.Add( monster->GetUID(), newData);
}

void UagMonsterMoveManager::_move(float elapsedTime)
{
	for( auto iterator = _movingContainer.CreateIterator(); iterator; ++iterator ){
		auto& movingData = iterator.Value();
		if( movingData._movingMonster == nullptr ){
			continue;
		}
		if( movingData._movingElapsedTime >= movingData._movingTotalTime ){
			movingData._movingMonster->MoveFinish();
			FRotator rotator;
			GetMovePointRotation( movingData._type, rotator);
			movingData._movingMonster->SetActorRotation( rotator );
			movingData._movingMonster = nullptr;
			iterator.RemoveCurrent();
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
