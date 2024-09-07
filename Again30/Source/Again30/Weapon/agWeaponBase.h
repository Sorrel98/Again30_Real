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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	float MoveSpeedRate;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	float AttackAnimPlayRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Properties", meta=(AllowPrivateAccess=true))
	bool bPhysicsWeapon;
	
private:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mesh", meta=(AllowPrivateAccess=true))
	FName WeaponAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	bool bNowDoingAttack;
	bool bAttacked;
	
public:
	AagWeaponBase();

	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName, ACharacter* Character);
	virtual void RemoveWeapon();

	virtual void DealDamageToTarget(IagDamageable* Target);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void WeaponAttackStart();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void WeaponAttackEnd();

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void DoAttack();
	

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	virtual void OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintReadOnly, Category="Weapon", meta=(AllowPrivateAccess=true))
	ACharacter* OwnerCharacter;
};
