#include "AagPlayGameMode.h"

#include "agGameModeExtraData.h"
#include "EngineUtils.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Again30/Monster/agMonsterBase.h"
#include "GameFramework/PlayerStart.h"

AagPlayGameMode::AagPlayGameMode()
	:
	GenerationTime(30.f), CurGeneration(1)
{
}

void AagPlayGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	_setManagerContainer();
}

void AagPlayGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AagPlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float PrevTime = CurGenerationTime;
	CurGenerationTime -= DeltaSeconds;

	_managerTick( DeltaSeconds );
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

bool AagPlayGameMode::GetManager(EagManagerType type, TObjectPtr<UagManagerBase>& manager )
{
	if( _managerContainer.Contains(type) )
	{
		manager = _managerContainer[type];
		return true;
	}
	return false;
}


int32 AagPlayGameMode::GetNewMonsterUID()
{
	++_monsterUID;
	return _monsterUID - 1;
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

void AagPlayGameMode::_setManagerContainer()
{
	// @todo 야매
	FSoftObjectPath extraDataPath = FSoftObjectPath( TEXT("/Script/Again30.agGameModeExtraData'/Game/Mode/DA_ModeExtraData.DA_ModeExtraData'"));
	_extraData = Cast<UagGameModeExtraData>(extraDataPath.TryLoad());
	if( _extraData != nullptr ){
		for( auto managerType : _extraData->ManagerList ){
			if( managerType == EagManagerType::None ){
				continue;
			}
			// @todo factory패턴으로 하고 싶었다.
			auto newManagerObject = _createManager( managerType );
			if( newManagerObject != nullptr )
			{
				newManagerObject->BeginPlay();
				_managerContainer.Add( managerType, newManagerObject );
			}
		}
	}
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

void AagPlayGameMode::_managerTick(float elapsedTime)
{
	for( auto& manager : _managerContainer ){
		if( manager.Value == nullptr ){
			continue;
		}
		manager.Value->Tick(elapsedTime);
	}
}
