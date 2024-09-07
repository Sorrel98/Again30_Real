// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterBase.h"

#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Animation/agAnimInstance.h"
#include "Animation/agMonsterActionBase.h"
#include "ExtraData/agMonsterExtraDataBase.h"


AagMonsterBase::AagMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

int32 AagMonsterBase::GetUID()
{
	return _uid;
}

void AagMonsterBase::SetUID(int32 uid)
{
	_uid = uid;
}

void AagMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	_initMonster();

	DealTiredDamage( 101, this );
}

// Called every frame
void AagMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if( _extraData == nullptr ){
		return;
	}
	if( _useHpBar == true ){
		_hpBarShow_ElapsedTime += DeltaTime;
		if( _hpBarShow_ElapsedTime >= _extraData->HpBarShowTime ){
			SetHpBarVisible( false );
			_hpBarShow_ElapsedTime = 0;
			_useHpBar = false;
		}
	}
	if( _tickDam == true ){
		_tickDam_ElapsedTime += DeltaTime;
		_tickDam_TotalElapsedTime += DeltaTime;
		if( _tickDam_ElapsedTime >= 1 ){
			_tickDam_ElapsedTime = 0.f;
			_attribute.HP -= _extraData->TickDam;
		}
		if( _tickDam_TotalElapsedTime >= _extraData->TickDamTime){
			_tickDam_ElapsedTime = 0;
			_tickDam_TotalElapsedTime = 0;
			_tickDam = false;
		}
	}
}

// Called to bind functionality to input
void AagMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AagMonsterBase::MoveMonster(EagMonsterMovePointType targetLocation)
{
	if( _attribute.state == EegMonsterState::Walking ){
		return;
	}
	TObjectPtr<UagMonsterMoveManager> monsterMoveManager;
	if( _getMoveManger(monsterMoveManager) == false){
		return;
	}
	if( monsterMoveManager == nullptr ){
		return;
	}
	monsterMoveManager->RequestMoveToPoint(this, EagMonsterMovePointType::Bed, 3);
	_attribute.state = EegMonsterState::Walking;
}

FVector AagMonsterBase::GetPointLocation(EagMonsterMovePointType locationPoint)
{
	TObjectPtr<class AagPlayGameMode> again30GameMode;
	if( _getAgGameMode(again30GameMode) == false){
		return FVector::ZeroVector;
	}
	FVector resultLocation = FVector::ZeroVector;
	if( again30GameMode->GetMovePointLocation(locationPoint, resultLocation) == false ){
		return FVector::ZeroVector;
	}
	return resultLocation;
}

void AagMonsterBase::MoveFinish()
{
	SetState( EegMonsterState::Idle );
}

EegMonsterState AagMonsterBase::GetState()
{
	return _attribute.state;
}

void AagMonsterBase::SetState(EegMonsterState state)
{
	_attribute.state = state;
}

float AagMonsterBase::GetHp()
{
	return _attribute.HP;
}

void AagMonsterBase::SetHpBarVisible_Implementation(bool isVisible)
{
}

void AagMonsterBase::SetInitMaxHpValue_Implementation(float maxValue)
{
}

FVector AagMonsterBase::GetDamageableActorLocation() const
{
	return GetActorLocation();
}

void AagMonsterBase::DealDamage(float DamageAmount, AActor* DamageCauser)
{
	if( _attribute.HP <= 0 ){
		return;
	}
	_attribute.HP -= DamageAmount;
	if( _attribute.HP <= 0 ){
		_monsterDead();
		return;
	}
	_playTakeDamage();
	SetHpBarVisible( true );
	_useHpBar = true;
}

void AagMonsterBase::DealTiredDamage(float DamageAmount, AActor* DamageCauser)
{
	if( _attribute.TiredGage <= 0 ){
		return;
	}
	_attribute.TiredGage -= DamageAmount;
	if( _attribute.TiredGage <= 0 ){
		_tickDam = true;
	}
}

void AagMonsterBase::_initMonster()
{
	_initLocation();
	_initAction();
	_initAttribute();

	//MoveMonster(EagMonsterMovePointType::Bed);
	//TakeDamage(10000);
}

void AagMonsterBase::_initLocation()
{
	if( _extraData == nullptr ){
		return;
	}
	_setMonsterLocation( _getMonsterLocation(_extraData->InitSpawnMovePoint) );
}

void AagMonsterBase::_initAction()
{
	if( _action == nullptr ){
		_action = NewObject<UagMonsterActionBase>();
	}
}

void AagMonsterBase::_initAttribute()
{
	if( _extraData == nullptr ){
		return;
	}
	_attribute.state = EegMonsterState::Idle;
	_attribute.HP = _extraData->MaxHp;
	_attribute.TiredGage = _extraData->MaxTiredGage;
	const auto gameMode = GetWorld()->GetAuthGameMode();
	if( gameMode == nullptr ){
		return;
	}
	const auto again30GameMode = Cast<AagPlayGameMode>(gameMode);
	if( again30GameMode == nullptr ){
		return;
	}
	SetUID(again30GameMode->GetNewMonsterUID());
	SetInitMaxHpValue(_extraData->MaxHp);
}

FVector AagMonsterBase::_getMonsterLocation(EagMonsterMovePointType pointType)
{
	TObjectPtr<class AagPlayGameMode> again30GameMode;
	if( _getAgGameMode(again30GameMode) == false){
		return FVector::ZeroVector;
	}
	if( again30GameMode == nullptr ){
		return FVector::ZeroVector;
	}
	FVector location = FVector::ZeroVector;
	if( again30GameMode->GetMovePointLocation(pointType, location) == true ){
		return location;
	}
	return FVector::ZeroVector;
}

void AagMonsterBase::_setMonsterLocation(const FVector& location)
{
	SetActorLocation(location);
}

bool AagMonsterBase::_getMoveManger( TObjectPtr<class UagMonsterMoveManager>& manager )
{
	TObjectPtr<class AagPlayGameMode> again30GameMode = nullptr;
	if( _getAgGameMode(again30GameMode) == false){
		return false;
	}
	if( again30GameMode == nullptr ){
		return false;
	}
	if( again30GameMode->GetManager(EagManagerType::MonsterMove, manager) == false ){
		return false;
	}
	return true;
}

bool AagMonsterBase::_getAgGameMode( TObjectPtr<class AagPlayGameMode>& again30GameMode )
{
	if( GetWorld() == nullptr ){
		return false;
	}
	const auto gameMode = GetWorld()->GetAuthGameMode();
	if( gameMode == nullptr ){
		return false;
	}
	again30GameMode = Cast<AagPlayGameMode>(gameMode);
	return true;
}

void AagMonsterBase::_monsterDead()
{
	TObjectPtr<AagPlayGameMode> again30GameMode = nullptr;
	if( _getAgGameMode(again30GameMode) == false ){
		return;
	}
	SetState(EegMonsterState::Corpse);
	auto mesh = GetMesh();
	if( mesh == nullptr ){
		return;
	}
	mesh->SetSimulatePhysics( true );
	mesh->SetCollisionProfileName(TEXT("Ragdoll"));
	mesh->SetAnimationMode(EAnimationMode::AnimationCustomMode);
	
	again30GameMode->FishWin();
}

void AagMonsterBase::_playTakeDamage()
{
	const auto& mesh = GetMesh();
	if( mesh == nullptr ){
		return;
	}
	const auto animInstance = mesh->GetAnimInstance();
	if( animInstance == nullptr ){
		return;
	}
	const auto agAnimInstance = Cast<UagAnimInstance>( animInstance );
	if( agAnimInstance == nullptr){
		return;
	}
	agAnimInstance->PlayTakeDamageMontage();
}