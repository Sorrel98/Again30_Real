#include "AagPlayGameMode.h"

#include "agGameModeExtraData.h"
#include "EngineUtils.h"
#include "Again30/Manager/UagMonsterMoveManager.h"
#include "GameFramework/PlayerStart.h"

AagPlayGameMode::AagPlayGameMode()
	:
	GenerationTime(30.f), CurGeneration(1)
{
}

void AagPlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	// @todo 야매
	FSoftObjectPath extraDataPath = FSoftObjectPath( TEXT("/Script/Again30.agGameModeExtraData'/Game/Mode/DA_ModeExtraData.DA_ModeExtraData'"));
	_extraData = Cast<UagGameModeExtraData>(extraDataPath.TryLoad());
	if ( _extraData != nullptr ){
		for ( auto managerType : _extraData->ManagerList ){
			if ( managerType == EagManagerType::None ){
				continue;
			}
			// @todo factory패턴으로 하고 싶었다.
			auto newManagerObject = _createManager(managerType);
			if (newManagerObject != nullptr)
			{
				newManagerObject->BeginPlay();
				_managerContainer.Add(managerType, newManagerObject);
			}
		}
	}
	
}

void AagPlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float PrevTime = CurGenerationTime;
	CurGenerationTime -= DeltaSeconds;
	
}

void AagPlayGameMode::GameStart()
{
}

void AagPlayGameMode::GenerationStart()
{
	CurGenerationTime = GenerationTime;
	
	SpawnFish();
	IncreaseGeneration();
}

void AagPlayGameMode::GenerationEnd()
{
}

void AagPlayGameMode::IncreaseGeneration()
{
	CurGeneration += 1;
	OnGenerationChanged.Broadcast(CurGeneration);
}

void AagPlayGameMode::SpawnFish()
{
	APlayerStart* StartPoint = GetPlayerStartPoint();
	if(StartPoint == nullptr || GetWorld() == nullptr) [[unlikely]]
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	APawn* FishPawn = GetWorld()->SpawnActor<APawn>(StartPoint->GetActorLocation(), StartPoint->GetActorRotation(), SpawnParameters);
	if(FishPawn)
	{
		
	}
}

APlayerStart* AagPlayGameMode::GetPlayerStartPoint()
{
	APlayerStart* FoundPlayerStart = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (IsValid(PlayerStart))
		{
			FoundPlayerStart = PlayerStart;
			break;
		}
	}
	return FoundPlayerStart;
}

TObjectPtr<UagManagerBase> AagPlayGameMode::_createManager(EagManagerType type)
{
	TObjectPtr<UagManagerBase> newManager = nullptr;
	switch (type){
	case EagManagerType::MonsterMove :{
		newManager = NewObject<UagMonsterMoveManager>();
		break;
	}	
	default: ;
	}
	return newManager;
}
