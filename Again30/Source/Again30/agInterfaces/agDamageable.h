// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "agDamageable.generated.h"

UINTERFACE()
class UagDamageable : public UInterface
{
	GENERATED_BODY()
};


class AGAIN30_API IagDamageable
{
	GENERATED_BODY()

public:
	virtual FVector GetDamageableActorLocation() const { return FVector::ZeroVector; }
	virtual void DealDamage(float DamageAmount, AActor* DamageCauser) { }
	virtual void DealTiredDamage(float DamageAmount, AActor* DamageCauser) { }
};
