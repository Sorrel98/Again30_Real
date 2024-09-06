
#include "agMonsterMovePoint.h"


AagMonsterMovePoint::AagMonsterMovePoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AagMonsterMovePoint::BeginPlay()
{
	Super::BeginPlay();

	//@todo Monster Move Manger 한테 TMap Container에 저장해서 관리해달라고 요청해야 함.
}

void AagMonsterMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

