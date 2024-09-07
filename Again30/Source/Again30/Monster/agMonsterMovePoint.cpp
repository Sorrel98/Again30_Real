
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

	if( GetWorld() == nullptr ){
		return;
	}
	const auto gameMode = Cast<AagPlayGameMode>(GetWorld()->GetAuthGameMode());
	if( gameMode == nullptr ){
		return;
	}
	TObjectPtr<UagMonsterMoveManager> manager = nullptr;
	if( gameMode->GetManager(EagManagerType::MonsterMove, manager) == false ){
		return;
	}
	if( manager != nullptr ){
		manager->AddMonsterMovePoint(_pointType, this);
	}
}


void AagMonsterMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EagMonsterMovePointType AagMonsterMovePoint::GetType()
{
	return _pointType;
}

