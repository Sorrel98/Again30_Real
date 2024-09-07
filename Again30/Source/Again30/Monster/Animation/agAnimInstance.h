// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "agAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AGAIN30_API UagAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void PlayTakeDamageMontage();

	UFUNCTION(BlueprintNativeEvent)
	void PlayDeadMontage();
};
