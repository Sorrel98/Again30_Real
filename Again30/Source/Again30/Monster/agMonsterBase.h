// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Library/agEnumClass.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "GameFramework/Character.h"
#include "agMonsterBase.generated.h"

UENUM( BlueprintType, Blueprintable)
enum class EegMonsterState :uint8
{
	None,
	Idle,
	Walking,
	Corpse,
	Max
};

USTRUCT()
struct FvMonsterAttribute
{
	GENERATED_BODY()

	UPROPERTY()
	float HP = 0.f;
	UPROPERTY()
	float TiredGage = 0.f;
	UPROPERTY()
	EegMonsterState state = EegMonsterState::None;
};


UCLASS()
class AGAIN30_API AagMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AagMonsterBase();
	int32 GetUID();
	void SetUID( int32 uid );

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// movement
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveMonster( EagMonsterMovePointType targetLocation );
	FVector GetPointLocation( EagMonsterMovePointType locationPoint );
	void MoveFinish();

	// state
	UFUNCTION(BlueprintCallable)
	EegMonsterState GetState();
	void SetState( EegMonsterState state );

	// damage
	// @todo 현재는 Damage float 값만 필요함
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void TakeDamage( float damage );

protected:
	void _initMonster();
	void _initLocation();
	void _initAction();
	void _initAttribute();

	// location
	FVector _getMonsterLocation(EagMonsterMovePointType pointType);
	void _setMonsterLocation(const FVector& location );

	// get
	bool _getAgGameMode(TObjectPtr<class AagPlayGameMode>& again30GameMode);
	bool _getMoveManger(TObjectPtr<class UagMonsterMoveManager>& manager);
	
	// dead
	void _monsterDead();

	int32 _uid = 0;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category="Again30")
	TObjectPtr<class UagMonsterExtraDataBase> _extraData;
	UPROPERTY()
	TObjectPtr<class UagMonsterActionBase> _action = nullptr;

	float _elapsedTime = 0.f;

	UPROPERTY()
	FvMonsterAttribute _attribute = FvMonsterAttribute();
};
