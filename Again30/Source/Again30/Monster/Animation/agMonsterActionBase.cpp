// Fill out your copyright notice in the Description page of Project Settings.


#include "agMonsterActionBase.h"

#include "agAnimInstance.h"
#include "Again30/Monster/agMonsterBase.h"

void UagMonsterActionBase::PlayMontage(const TObjectPtr<class AagMonsterBase>& monster, TObjectPtr<UAnimMontage> montage)
{
	if( monster == nullptr || montage == nullptr ){
		return;
	}
	const auto& mesh = monster->GetMesh();
	if( mesh == nullptr ){
		return;
	}
	const auto animInstance = mesh->GetAnimInstance();
	if( animInstance == nullptr ){
		return;
	}
	const auto agAnimInstance = Cast<UagAnimInstance>( animInstance );
	if( agAnimInstance == nullptr){
		return;
	}
	agAnimInstance->PlayTakeDamageMontage();
}

void UagMonsterActionBase::PlayMontage(const TObjectPtr<class AagMonsterBase>& monster, const FSoftObjectPath montagePath)
{
	if( monster == nullptr || montagePath.IsValid() == false ){
		return;
	}
	auto loadedObject = montagePath.TryLoad();
	if( loadedObject == nullptr ){
		return;
	}
	TObjectPtr<UAnimMontage> animMontage = Cast<UAnimMontage>(loadedObject);
	if( animMontage == nullptr ){
		return;
	}
	PlayMontage(monster, animMontage);
}