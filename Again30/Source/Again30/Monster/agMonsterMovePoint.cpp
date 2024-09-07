
#include "agMonsterMovePoint.h"

#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Manager/agMonsterMoveManager.h"


AagMonsterMovePoint::AagMonsterMovePoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AagMonsterMovePoint::BeginPlay()
{
	Super::BeginPlay();
	_registerContainer();
}

void AagMonsterMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EagMonsterMovePointType AagMonsterMovePoint::GetType()
{
	return _pointType;
}

bool AagMonsterMovePoint::_registerContainer()
{
	const auto gameMode = Cast<AagPlayGameMode>(GetWorld()->GetAuthGameMode());
	if( gameMode == nullptr ){
		return true;
	}
	gameMode->RegisterMovePoint(_pointType, this);
	return false;
}