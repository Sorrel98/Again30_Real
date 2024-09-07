#include "AagAreaDamageWeaponBase.h"
#include "Again30/agInterfaces/agDamageable.h"
#include "Again30/Fish/agFish.h"
#include "Again30/Monster/agMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AagAreaDamageWeaponBase::AagAreaDamageWeaponBase()
	:
	DamageOffset({0.f, 0.f, 0.f}),
	DamageExtent({100.f, 100.f, 10.f}),
	DamageInterval(3.f),
	bDamageAreaActivated(true),
	ModifySpeed(1.f), ModifyJumpPower(1.f), ModifySearchInterval(0.1f)
{
	PrimaryActorTick.bCanEverTick = true;	
}

void AagAreaDamageWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(TargetMonster == nullptr)
	{
		TargetMonster = Cast<AagMonsterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AagMonsterBase::StaticClass()));
	}

	// Modifier 는 0.1초마다 검사 예정
	if(bIsModifierSpringboard && GetWorld())
	{
		if(AagPlayGameMode* GameMode = GetWorld()->GetAuthGameMode<AagPlayGameMode>())
		{
			GameMode->OnGenerationEndEvent.AddUObject(this, &AagAreaDamageWeaponBase::ResetModifier);
		}
		GetWorld()->GetTimerManager().SetTimer(ModifyTimer, this, &AagAreaDamageWeaponBase::ModifyPlayer, ModifySearchInterval, true);
	}
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

void AagAreaDamageWeaponBase::EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName)
{
	Super::EquipWeapon(SkeletalToAttach, AttackSocketName);

	ToggleDamageArea();
}

void AagAreaDamageWeaponBase::RemoveWeapon()
{
	Super::RemoveWeapon();
	
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
		if(DamageAreaBox.IsInsideXY(TargetMonster->GetDamageableActorLocation()))
		{
			DealDamageToTarget(TargetMonster.GetInterface());
		}
	}
}

void AagAreaDamageWeaponBase::ModifyPlayer()
{
	if(bIsModifierSpringboard == false || bModified)
	{
		return;
	}
	if(GetWorld() == nullptr) [[unlikely]]
	{
		return;
	}
	AagPlayGameMode* GameMode = GetWorld()->GetAuthGameMode<AagPlayGameMode>();
	if(GameMode && GameMode->GetCurrentFish())
	{
		const FBox DamageAreaBox = FBox::BuildAABB(GetActorLocation() + DamageOffset, DamageExtent);
		if(DamageAreaBox.IsInsideXY(GameMode->GetCurrentFish()->GetActorLocation()))
		{
			GameMode->GetCurrentFish()->MultiplyHopForce(ModifySpeed);
			GameMode->GetCurrentFish()->GetCharacterMovement()->JumpZVelocity *= ModifyJumpPower;
			bModified = true;
		}
	}
}
