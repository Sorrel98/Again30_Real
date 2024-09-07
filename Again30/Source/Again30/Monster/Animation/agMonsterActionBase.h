// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "agMonsterActionBase.generated.h"

/**
 * 
 */
UCLASS()
class AGAIN30_API UagMonsterActionBase : public UObject
{
	GENERATED_BODY()
public:
	void PlayMontage(const TObjectPtr<class AagMonsterBase>& monster, TObjectPtr<UAnimMontage> montage);
	void PlayMontage( const TObjectPtr<class AagMonsterBase>& monster, const FSoftObjectPath montagePath );

protected:
	
};
