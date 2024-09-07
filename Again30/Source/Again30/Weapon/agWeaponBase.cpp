﻿#include "agWeaponBase.h"

#include "Again30/agInterfaces/agDamageable.h"
#include "Kismet/GameplayStatics.h"


AagWeaponBase::AagWeaponBase()
	:
	WeaponDamage(0.f), WeaponTiredDamage(0.f),
	WeaponAttachSocketName(TEXT("WeaponAttach")),
	bNowDoingAttack(true)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	if(WeaponMesh)
	{
		WeaponMesh->SetupAttachment(GetRootComponent());
	}
}

void AagWeaponBase::EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if(WeaponMesh && SkeletalToAttach)
	{
		WeaponMesh->AttachToComponent(SkeletalToAttach, AttachmentRules, AttackSocketName);
	}
}

void AagWeaponBase::RemoveWeapon()
{
	if(WeaponMesh)
	{
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AagWeaponBase::DealDamageToTarget(IagDamageable* Target)
{
	if(Target)
	{
		if(bAttackHP)
		{
			Target->DealDamage(WeaponDamage, this);
		}

		if(bAttackTired)
		{
			Target->DealTiredDamage(WeaponTiredDamage, this);
		}
	}
}

void AagWeaponBase::WeaponAttackStart()
{
	bNowDoingAttack = true;
	bAttacked = false;
}

void AagWeaponBase::WeaponAttackEnd()
{
	bNowDoingAttack = false;
	bAttacked = true;
}

void AagWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if(WeaponMesh)
	{
		WeaponMesh->OnComponentHit.AddDynamic(this, &AagWeaponBase::OnWeaponHit);
	}
}

void AagWeaponBase::OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(bNowDoingAttack && bAttacked == false)
	{
		UGameplayStatics::ApplyDamage(OtherActor, WeaponDamage, GetWorld()->GetFirstPlayerController(), GetOwner(), UDamageType::StaticClass());
		bAttacked = true;
	}
}
