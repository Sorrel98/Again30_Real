
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

	if( GetWorld() == nullptr )
	{
		return;
	}
	const auto gameMode = Cast<AagPlayGameMode>(GetWorld()->GetAuthGameMode());
	if( gameMode != nullptr )
	{
		TObjectPtr<UagManagerBase> manager = nullptr;
		if( gameMode->GetManager(EagManagerType::MonsterMove, manager) == true )
		{
			TObjectPtr<UagMonsterMoveManager> moveManager = Cast<UagMonsterMoveManager>(manager);
			if( moveManager != nullptr )
			{
				moveManager->AddMonsterMovePoint(_pointType, this);
			}
		}
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

