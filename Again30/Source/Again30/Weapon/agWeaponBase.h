// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "agWeaponBase.generated.h"

class IagDamageable;
class UStaticMeshComponent;

UCLASS()
class AGAIN30_API AagWeaponBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	float WeaponDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	float WeaponTiredDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bAttackHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bAttackTired;
	
private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh", meta=(AllowPrivateAccess=true))
	FName WeaponAttachSocketName;

	bool bNowDoingAttack;
	bool bAttacked;
	
public:
	AagWeaponBase();

	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName);
	virtual void RemoveWeapon();

	virtual void DealDamageToTarget(IagDamageable* Target);
	
	void WeaponAttackStart();
	void WeaponAttackEnd();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
