#include "UFootstepAnimNotify.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

UUFootstepAnimNotify::UUFootstepAnimNotify()
	:
	CheckDistance(20.f)
{
}

void UUFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if(MeshComp == nullptr || MeshComp->GetWorld() == nullptr)	return;

	FHitResult HitResult;
	const FVector FootLocation = MeshComp->GetSocketLocation(BoneName);
	const FVector End{ FootLocation + FVector{0.f, 0.f, -100.f } };
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());

	auto World = MeshComp->GetWorld();
	World->LineTraceSingleByChannel(HitResult, FootLocation, End, ECC_WorldStatic, QueryParams);
	if(HitResult.bBlockingHit && HitResult.Distance < CheckDistance)
	{
		UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), FootStepSound, FootLocation, FRotator::ZeroRotator, VolumeMultiplier);
	}
}
