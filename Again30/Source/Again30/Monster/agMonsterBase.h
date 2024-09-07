// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/agInterfaces/agDamageable.h"
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
class AGAIN30_API AagMonsterBase : public ACharacter, public IagDamageable
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
	UFUNCTION(BlueprintCallable, BlueprintType, Blueprintable)
	float GetHp();
	UFUNCTION(BlueprintNativeEvent)
	void SetHpBarVisible( bool isVisible );
	UFUNCTION(BlueprintNativeEvent)
	void SetInitMaxHpValue( float maxValue );
	
	// IagDamageable
	virtual FVector GetDamageableActorLocation() const override;
	virtual void DealDamage(float DamageAmount, AActor* DamageCauser) override;
	virtual void DealTiredDamage(float DamageAmount, AActor* DamageCauser) override;

protected:
	// init
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

	// monatge
	void _playTakeDamage();

	int32 _uid = 0;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category="Again30")
	TObjectPtr<class UagMonsterExtraDataBase> _extraData;
	UPROPERTY()
	TObjectPtr<class UagMonsterActionBase> _action = nullptr;

	UPROPERTY()
	FvMonsterAttribute _attribute = FvMonsterAttribute();

	// hp bar
	UPROPERTY()
	TObjectPtr<UUserWidget> _hpBar = nullptr;
	float _elapsedTime = 0.f; // @todo remove
	bool _useHpBar = false;
	float _hpBarShow_ElapsedTime = 0.f;
};
