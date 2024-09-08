#include "agWeaponBase.h"

#include "Again30/agInterfaces/agDamageable.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


AagWeaponBase::AagWeaponBase()
	:
	WeaponDamage(0.f), WeaponTiredDamage(0.f),
	WeaponAttachSocketName(TEXT("WeaponAttach")),
	bNowDoingAttack(true),
	MoveSpeedRate(1.f), AttackAnimPlayRate(1.f)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	if(WeaponMesh)
	{
		WeaponMesh->SetupAttachment(GetRootComponent());
	}
}

void AagWeaponBase::EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName, ACharacter* Character)
{
	OwnerFish = Character;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if(WeaponMesh && SkeletalToAttach)
	{
		WeaponMesh->AttachToComponent(SkeletalToAttach, AttachmentRules, AttackSocketName);
	}
	ACharacter* OwnerCharacter =  Cast<ACharacter>(SkeletalToAttach->GetOwner());
	if(OwnerCharacter)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed *= MoveSpeedRate;
	}

	if(bPhysicsWeapon)
	{
		bAttacked = false;
	}
}

void AagWeaponBase::RemoveWeapon()
{
	if(WeaponMesh)
	{
		ACharacter* OwnerCharacter =  Cast<ACharacter>(WeaponMesh->GetAttachmentRoot()->GetOwner());
		if(OwnerCharacter)
		{
			OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed /= MoveSpeedRate;
		}
		WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if(bPhysicsWeapon)
	{
		WeaponMesh->SetSimulatePhysics(true);
		WeaponMesh->SetNotifyRigidBodyCollision(true);
		WeaponMesh->OnComponentHit.AddDynamic(this, &AagWeaponBase::OnWeaponHit);
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

void AagWeaponBase::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bNowDoingAttack && bAttacked == false)
	{
		if(IagDamageable* Damageable = Cast<IagDamageable>(OtherActor))
		{
			if(WeaponHitSoundCue)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponHitSoundCue, GetActorLocation(), FRotator::ZeroRotator, HitVolumeMultiplier);
			}
			Damageable->DealDamage(WeaponDamage, this);
			bAttacked = true;
		}
	}
}

void AagWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if(WeaponMesh)
	{
		if (bPhysicsWeapon)
		{
			WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
		else
		{
			WeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AagWeaponBase::OnWeaponOverlap);
			WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
		
	}
}

void AagWeaponBase::OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(bPhysicsWeapon && bAttacked == false)
	{
		if(IagDamageable* Damageable = Cast<IagDamageable>(OtherActor))
		{
			if(WeaponHitSoundCue)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponHitSoundCue, Hit.Location, FRotator::ZeroRotator, HitVolumeMultiplier);
			}
			Damageable->DealDamage(WeaponDamage, this);
			bAttacked = true;
		}
	}
}
