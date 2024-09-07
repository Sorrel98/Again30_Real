// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "GameFramework/Character.h"
#include "agMonsterBase.generated.h"

UENUM( BlueprintType, Blueprintable)
enum class EegMonsterState :uint8
{
	None,
	Idle,
	Moving,
	Max
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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveMonster( EagMonsterMovePointType targetLocation );
	FVector GetPointLocation( EagMonsterMovePointType locationPoint );


protected:
	void _initMonster();
	void _initAction();
	void _initAttribute();

	FVector _getMonsterLocation(EagMonsterMovePointType pointType);
	void _setMonsterLocation(const FVector& location );

	int32 _uid = 0;

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category="Again30")
	TObjectPtr<class UagMonsterExtraDataBase> _extraData;
	UPROPERTY()
	TObjectPtr<class UagMonsterActionBase> _action = nullptr;

	float _elapsedTime = 0.f;

	UPROPERTY()
	EegMonsterState _state = EegMonsterState::None;
};
