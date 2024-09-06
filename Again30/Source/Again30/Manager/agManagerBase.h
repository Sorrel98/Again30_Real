// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "agManagerBase.generated.h"

/**
 * 
 */
UCLASS()
class AGAIN30_API UagManagerBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void BeginPlay();
};
