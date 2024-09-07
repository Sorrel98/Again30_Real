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

	_initMonster();
}

// Called every frame
void AagMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if( _attribute.state != EegMonsterState::Moving ){
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
	if( _attribute.state == EegMonsterState::Moving ){
		return;
	}
	TObjectPtr<UagMonsterMoveManager> monsterMoveManager;
	if( _getMoveManger(monsterMoveManager) == false){
		return;
	}
	if( monsterMoveManager == nullptr ){
		return;
	}
	monsterMoveManager->RequestMoveToPoint(this, EagMonsterMovePointType::Bed, 3);
	_attribute.state = EegMonsterState::Moving;
}

FVector AagMonsterBase::GetPointLocation(EagMonsterMovePointType locationPoint)
{
	TObjectPtr<class AagPlayGameMode> again30GameMode;
	if( _getAgGameMode(again30GameMode) == false){
		return FVector::ZeroVector;
	}
	FVector resultLocation = FVector::ZeroVector;
	if( again30GameMode->GetMovePointLocation(locationPoint, resultLocation) == false ){
		return FVector::ZeroVector;
	}
	return resultLocation;
}

void AagMonsterBase::_initMonster()
{
	_initLocation();
	_initAction();
	_initAttribute();
}

void AagMonsterBase::_initLocation()
{
	if( _extraData == nullptr ){
		return;
	}
	_setMonsterLocation( _getMonsterLocation(_extraData->InitSpawnMovePoint) );
}

void AagMonsterBase::_initAction()
{
	if( _action == nullptr ){
		_action = NewObject<UagMonsterActionBase>();
	}
}

void AagMonsterBase::_initAttribute()
{
	if( _extraData == nullptr ){
		return;
	}
	_attribute.state = EegMonsterState::Idle;
	_attribute.HP = _extraData->InitHp;
	_attribute.TiredGage = _extraData->InitTiredGage;
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
	TObjectPtr<class AagPlayGameMode> again30GameMode;
	if( _getAgGameMode(again30GameMode) == false){
		return FVector::ZeroVector;
	}
	if( again30GameMode == nullptr ){
		return FVector::ZeroVector;
	}
	FVector location = FVector::ZeroVector;
	if( again30GameMode->GetMovePointLocation(pointType, location) == true ){
		return location;
	}
	return FVector::ZeroVector;
}

void AagMonsterBase::_setMonsterLocation(const FVector& location)
{
	SetActorLocation(location);
}

bool AagMonsterBase::_getMoveManger( TObjectPtr<class UagMonsterMoveManager>& manager )
{
	TObjectPtr<class AagPlayGameMode> again30GameMode = nullptr;
	if( _getAgGameMode(again30GameMode) == false){
		return false;
	}
	if( again30GameMode == nullptr ){
		return false;
	}
	if( again30GameMode->GetManager(EagManagerType::MonsterMove, manager) == false ){
		return false;
	}
	return true;
}

bool AagMonsterBase::_getAgGameMode( TObjectPtr<class AagPlayGameMode>& again30GameMode )
{
	if( GetWorld() == nullptr ){
		return false;
	}
	const auto gameMode = GetWorld()->GetAuthGameMode();
	if( gameMode == nullptr ){
		return false;
	}
	again30GameMode = Cast<AagPlayGameMode>(gameMode);
	return true;
}