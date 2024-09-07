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

	/** 사실 인터페이스를 띄우지만 이것을 게임잼이기 때문에 Mdofier 발판도 이곳에 남깁니다 캬~ */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Springboard", meta=(AllowPrivateAccess=true))
	bool bIsModifierSpringboard;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Springboard", meta=(AllowPrivateAccess=true))
	bool bModified;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Springboard", meta=(AllowPrivateAccess=true))
	float ModifySpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Springboard", meta=(AllowPrivateAccess=true))
	float ModifyJumpPower;
	FTimerHandle ModifyTimer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Springboard", meta=(AllowPrivateAccess=true))
	float ModifySearchInterval;
	
	// Caching
	UPROPERTY()
	TScriptInterface<IagDamageable> TargetMonster;
	
public:
	AagAreaDamageWeaponBase();

	virtual void Tick(float DeltaSeconds) override;

	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName) override;
	virtual void RemoveWeapon() override;

	void ResetModifier() { bModified = false; }

protected:
	virtual void BeginPlay() override;

private:
	void ToggleDamageArea();
	void DealDamageToPlayer();
	void ModifyPlayer();
};
