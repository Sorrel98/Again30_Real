#pragma once

#include "CoreMinimal.h"
#include "agWeaponBase.h"
#include "AagAreaDamageWeaponBase.generated.h"

class UBoxComponent;

UCLASS()
class AGAIN30_API AagAreaDamageWeaponBase : public AagWeaponBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Damage Area", meta=(AllowPrivateAccess=true))
	TObjectPtr<UBoxComponent> DamageArea;

	bool bToggleDamageArea;
	
public:
	AagAreaDamageWeaponBase();

	virtual void EquipWeapon(USkeletalMeshComponent* SkeletalToAttach, FName AttackSocketName) override;
	virtual void RemoveWeapon() override;

protected:
	virtual void BeginPlay() override;

private:
	void ToggleDamageArea();
	
};
