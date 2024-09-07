// Fill out your copyright notice in the Description page of Project Settings.


#include "agNotifyState_MotionWarping.h"

#include "Again30/Monster/agMonsterBase.h"

void UagNotifyState_MotionWarping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if( MeshComp == nullptr || Animation == nullptr )
	{
		return;
	}
	auto owner = Cast<AagMonsterBase>( MeshComp->GetOwner());
	if( owner == nullptr )
	{
		return;
	}
	_totalDuration = TotalDuration;
	
	if( UseTargetLocation ){
		_targetLocation = TargetLocation;
	}
	if( UseTargetPointType ){
		
		_targetLocation = owner->GetPointLocation(TargetPointType);
	}
	_targetLocation += FVector( 0, 0, 88);
	_startLocation = owner->GetActorLocation();
	_beginPlayFinished = true;
	_motionElapsedTime = 0.f;
}

void UagNotifyState_MotionWarping::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if( _beginPlayFinished == false )
	{
		return;
	}
	if( !UseTargetLocation && !UseTargetPointType )
	{
		return;
	}
	if( MeshComp == nullptr || Animation == nullptr ){
		return;
	}
	auto movingActor = Cast<AActor>(MeshComp->GetOwner());
	if( movingActor == nullptr ){
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *FMath::Lerp(_startLocation, _targetLocation, FMath::Clamp(_motionElapsedTime / _totalDuration, 0, 1)).ToString())
	UE_LOG(LogTemp, Log, TEXT("%f"), FMath::Clamp(_motionElapsedTime / _totalDuration, 0, 1));
	movingActor->SetActorLocation(FMath::Lerp(_startLocation, _targetLocation, FMath::Clamp(_motionElapsedTime / _totalDuration, 0, 1)));
	_motionElapsedTime += FrameDeltaTime;
}

void UagNotifyState_MotionWarping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}