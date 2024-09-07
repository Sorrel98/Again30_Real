// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Again30/Library/agEnumClass.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "agNotifyState_MotionWarping.generated.h"

/**
 * 
 */
UCLASS( Blueprintable, BlueprintType)
class UagNotifyState_MotionWarping : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Blueprintable, Category = "Again30", meta=(EditCondition="!UseTargetPointType"))
	bool UseTargetLocation = false;
	UPROPERTY(EditAnywhere, Blueprintable, Category = "Again30", meta=(EditCondition="UseTargetLocation"))
	FVector TargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Blueprintable, Category = "Again30", meta=(EditCondition="!UseTargetLocation"))
	bool UseTargetPointType = false;
	UPROPERTY(EditAnywhere, Blueprintable, Category = "Again30", meta=(EditCondition="UseTargetPointType"))
	EagMonsterMovePointType TargetPointType = EagMonsterMovePointType::None;

	bool _beginPlayFinished = false;
	float _totalDuration = 0.f;
	float _motionElapsedTime = 0.f;

	FVector _startLocation = FVector::ZeroVector;
	FVector _targetLocation = FVector::ZeroVector;
};
