// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterBase.h"

#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "ExtraData/agMonsterExtraDataBase.h"


AagMonsterBase::AagMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AagMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if( _extraData == nullptr )
	{
		return;
	}
	_initMonster();

}

// Called every frame
void AagMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AagMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AagMonsterBase::_initMonster()
{
	_setMonsterLocation( _getMonsterLocation(_extraData->InitSpawnMovePoint) );
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
		TObjectPtr<UagManagerBase> manager = nullptr;
		if( again30GameMode->GetManager(EagManagerType::MonsterMove, manager) == false ){
			return FVector::ZeroVector;
		}
		if( manager == nullptr ){
			return FVector::ZeroVector;
		}
		const auto monsterMoveManager = Cast<UagMonsterMoveManager>(manager);
		if( monsterMoveManager == nullptr ){
			return FVector::ZeroVector;
		}
		FVector location = FVector::ZeroVector;
		if( monsterMoveManager->GetMovePointLocation(pointType, location) == true ){
			return location;
		}
	}
	return FVector::ZeroVector;
}

void AagMonsterBase::_setMonsterLocation(const FVector& location)
{
	SetActorLocation(location);
}