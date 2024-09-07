#include "AagAreaDamageWeaponBase.h"
#include "Again30/agInterfaces/agDamageable.h"
#include "Again30/Monster/agMonsterBase.h"
#include "Kismet/GameplayStatics.h"

AagAreaDamageWeaponBase::AagAreaDamageWeaponBase()
	:
	DamageOffset({0.f, 0.f, 0.f}),
	DamageExtent({100.f, 100.f, 10.f}),
	DamageInterval(3.f),
	bDamageAreaActivated(true)
{
	PrimaryActorTick.bCanEverTick = true;	
}

void AagAreaDamageWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bDamageAreaActivated)
	{
		ElapsedTime += DeltaSeconds;
		if(ElapsedTime > DamageInterval)
		{
			UE_LOG(LogTemp, Warning, TEXT("*** *** ** Call Damage"));
			ElapsedTime = 0.f;
			DealDamageToPlayer();
		}	
	}
}

void AagAreaDamageWeaponBase::EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName, ACharacter* Character)
{
	Super::EquipWeapon(SkeletalToAttach, AttackSocketName, Character);

	ToggleDamageArea();
}

void AagAreaDamageWeaponBase::RemoveWeapon()
{
	Super::RemoveWeapon();
	
}

void AagAreaDamageWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(TargetMonster == nullptr)
	{
		TargetMonster = Cast<AagMonsterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AagMonsterBase::StaticClass()));
	}
	
}

void AagAreaDamageWeaponBase::ToggleDamageArea()
{
	bDamageAreaActivated = !bDamageAreaActivated;
}

void AagAreaDamageWeaponBase::DealDamageToPlayer()
{
	if(bShowBoxArea)
	{
		UE_LOG(LogTemp, Warning, TEXT("*** *** ** Draw Box"));
		DrawDebugBox(GetWorld(), GetActorLocation() + DamageOffset, DamageExtent,
			FColor::Red, true, -1.f, 0, 5.f
		);
	}
	
	if(TargetMonster)
	{
		const FBox DamageAreaBox = FBox::BuildAABB(GetActorLocation() + DamageOffset, DamageExtent);
		if(DamageAreaBox.IsInside(TargetMonster->GetDamageableActorLocation()))
		{
			DealDamageToTarget(TargetMonster.GetInterface());
		}
	}
}