#include "AagAreaDamageWeaponBase.h"

AagAreaDamageWeaponBase::AagAreaDamageWeaponBase()
	:
	bToggleDamageArea(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AagAreaDamageWeaponBase::EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName)
{
	Super::EquipWeapon(SkeletalToAttach, AttackSocketName);

	ToggleDamageArea();
}

void AagAreaDamageWeaponBase::RemoveWeapon()
{
	Super::RemoveWeapon();
	
}

void AagAreaDamageWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AagAreaDamageWeaponBase::ToggleDamageArea()
{
	bToggleDamageArea = !bToggleDamageArea;
}
