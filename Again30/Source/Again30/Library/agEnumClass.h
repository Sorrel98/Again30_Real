
#pragma once

UENUM( BlueprintType, Blueprintable)
enum class EagMonsterMovePointType :uint8
{
	None,
	Desk,
	Bed,
	Kitchen,
	ElectricityController,
	Max
};


UENUM( BlueprintType, Blueprintable)
enum class EagManagerType :uint8
{
	None,
	MonsterMove,
	Max
};

#define ag_ObjectChannel_Fish	ECC_GameTraceChannel1
#define ag_ObjectChannel_Weapon	ECC_GameTraceChannel2
