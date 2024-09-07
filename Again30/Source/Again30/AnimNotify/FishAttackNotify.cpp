#include "FishAttackNotify.h"
#include "Again30/Fish/agFish.h"
#include "Again30/Weapon/agWeaponBase.h"

void UFishAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr || MeshComp->GetWorld() == nullptr)	return;

	if(const AagFish* Fish = Cast<AagFish>(MeshComp->GetOwner()))
	{
		if(AagWeaponBase* WeaponBase = Fish->GetCurrentWeapon())
		{
			if(bAttackStart)
			{
				WeaponBase->WeaponAttackStart();	
			}
			else
			{
				WeaponBase->WeaponAttackEnd();
			}
			
		}
	}
}
