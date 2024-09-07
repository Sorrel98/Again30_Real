// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterBase.h"

#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Animation/agMonsterActionBase.h"
#include "ExtraData/agMonsterExtraDataBase.h"


AagMonsterBase::AagMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

int32 AagMonsterBase::GetUID()
{
	return _uid;
}

void AagMonsterBase::SetUID(int32 uid)
{
	_uid = uid;
}

void AagMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if( _extraData == nullptr )
	{
		return;
	}
	_initMonster();
	_initAction();
	_initAttribute();
}

// Called every frame
void AagMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if( _state != EegMonsterState::Moving ){
		if( _elapsedTime > 5 ){
			MoveMonster(EagMonsterMovePointType::Bed);
		}
		else{
			_elapsedTime += DeltaTime;
		}
	}
}

// Called to bind functionality to input
void AagMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AagMonsterBase::MoveMonster(EagMonsterMovePointType targetLocation)
{
	if( _state == EegMonsterState::Moving ){
		return;
	}
	auto mode = GetWorld()->GetAuthGameMode();
	if( mode == nullptr ){
		return;
	}
	auto agMode = Cast<AagPlayGameMode>(mode);
	if( agMode == nullptr ){
		return;
	}
	TObjectPtr<UagMonsterMoveManager> monsterMoveManager;
	agMode->GetManager(EagManagerType::MonsterMove, monsterMoveManager);
	if( monsterMoveManager == nullptr ){
		return;
	}
	monsterMoveManager->RequestMoveToPoint(this, EagMonsterMovePointType::Bed, 3);
	_state = EegMonsterState::Moving;
}

FVector AagMonsterBase::GetPointLocation(EagMonsterMovePointType locationPoint)
{
	auto mode = GetWorld()->GetAuthGameMode();
	if( mode == nullptr ){
		return FVector::ZeroVector;
	}
	auto agMode = Cast<AagPlayGameMode>(mode);
	if( agMode == nullptr ){
		return FVector::ZeroVector;
	}
	TObjectPtr<UagMonsterMoveManager> monsterMoveManager;
	agMode->GetManager(EagManagerType::MonsterMove, monsterMoveManager);
	if( monsterMoveManager == nullptr ){
		return FVector::ZeroVector;
	}
	FVector resultLocation = FVector::ZeroVector;
	if( monsterMoveManager->GetMovePointLocation(locationPoint, resultLocation) == false ){
		return FVector::ZeroVector;
	}
	return resultLocation;
}

void AagMonsterBase::_initMonster()
{
	_setMonsterLocation( _getMonsterLocation(_extraData->InitSpawnMovePoint) );
}

void AagMonsterBase::_initAction()
{
	if( _action == nullptr )
	{
		_action = NewObject<UagMonsterActionBase>();
	}
}

void AagMonsterBase::_initAttribute()
{
	_state = EegMonsterState::Idle;
	const auto gameMode = GetWorld()->GetAuthGameMode();
	if( gameMode == nullptr ){
		return;
	}
	const auto again30GameMode = Cast<AagPlayGameMode>(gameMode);
	if( again30GameMode == nullptr ){
		return;
	}
	SetUID(again30GameMode->GetNewMonsterUID());
}

FVector AagMonsterBase::_getMonsterLocation(EagMonsterMovePointType pointType)
{
	if( GetWorld() != nullptr )
	{
		const auto gameMode = GetWorld()->GetAuthGameMode();
		if( gameMode == nullptr ){
			return FVector::ZeroVector;
		}
		const auto again30GameMode = Cast<AagPlayGameMode>(gameMode);
		if( again30GameMode == nullptr ){
			return FVector::ZeroVector;
		}
		TObjectPtr<UagMonsterMoveManager> manager = nullptr;
		if( again30GameMode->GetManager(EagManagerType::MonsterMove, manager) == false ){
			return FVector::ZeroVector;
		}
		if( manager == nullptr ){
			return FVector::ZeroVector;
		}
		FVector location = FVector::ZeroVector;
		if( manager->GetMovePointLocation(pointType, location) == true ){
			return location;
		}
	}
	return FVector::ZeroVector;
}

void AagMonsterBase::_setMonsterLocation(const FVector& location)
{
	SetActorLocation(location);
}