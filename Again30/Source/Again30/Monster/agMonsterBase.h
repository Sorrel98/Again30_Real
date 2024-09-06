// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "GameFramework/Character.h"
#include "agMonsterBase.generated.h"

UCLASS()
class AGAIN30_API AagMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AagMonsterBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	void _initMonster();

	FVector _getMonsterLocation(EagMonsterMovePointType pointType);
	void _setMonsterLocation(const FVector& location );

	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite, Category="Again30")
	TObjectPtr<class UagMonsterExtraDataBase> _extraData;
};
