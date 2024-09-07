#pragma once

#include "CoreMinimal.h"
#include "agWeaponBase.h"
#include "AagAreaDamageWeaponBase.generated.h"

class UBoxComponent;
class IagDamageable;

UCLASS()
class AGAIN30_API AagAreaDamageWeaponBase : public AagWeaponBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	FVector DamageOffset;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	FVector DamageExtent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	float DamageInterval;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	float ElapsedTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	bool bShowBoxArea;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	bool bDamageAreaActivated;

	// Caching
	UPROPERTY()
	TScriptInterface<IagDamageable> TargetMonster;
	
public:
	AagAreaDamageWeaponBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName, ACharacter* Character) override;
	virtual void RemoveWeapon() override;

protected:
	virtual void BeginPlay() override;

private:
	void ToggleDamageArea();
	void DealDamageToPlayer();
};
