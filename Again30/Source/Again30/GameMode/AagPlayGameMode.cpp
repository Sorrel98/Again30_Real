#include "AagPlayGameMode.h"
#include "agGameModeExtraData.h"
#include "EngineUtils.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Again30/Monster/agMonsterBase.h"
#include "Again30/Fish/agFish.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Again30/Monster/agMonsterMovePoint.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AagPlayGameMode::AagPlayGameMode()
	:
	GenerationTime(10.f), CurGeneration(1)
{
	CurGenerationTime = GenerationTime;
}

void AagPlayGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	_setManagerContainer();
}

APawn* AagPlayGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	return nullptr;
}

APawn* AagPlayGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	return nullptr;
}

void AagPlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameStart();
}

void AagPlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float PrevTime = CurGenerationTime;
	CurGenerationTime -= DeltaSeconds;

	_managerTick( DeltaSeconds );
	CalculateGenerationTime(DeltaSeconds);
}

void AagPlayGameMode::CalculateGenerationTime(float DeltaSeconds)
{
	// 물고기 사망-스폰 연출 중에는 시간 계산을 하지 않는다
	if(bNowDoingFishProduction) [[unlikely]]
	{
		return;
	}
	
	CurGenerationTime -= DeltaSeconds;
	OnCurrentTimeChanged.Broadcast(CurGenerationTime);
	if(CurGenerationTime < 0.f)
	{
		if(bNowDoingFishProduction == false)
		{
			GenerationEnd();
		}
	}
}

void AagPlayGameMode::GameStart()
{
	SpawnFish();
}

void AagPlayGameMode::GenerationStart()
{
	if(bNowDoingFishProduction == false)
	{
		return;
	}
	
	bNowDoingFishProduction = false;
	CurGenerationTime = GenerationTime;
	IncreaseGeneration();
}

void AagPlayGameMode::GenerationEnd()
{
	GetWorld()->GetFirstPlayerController()->UnPossess();
	if(CurrentFish)
	{
		CurrentFish->PlayFishDeadProduction();
	}
	
	bNowDoingFishProduction = true;
	CurGenerationTime = 0.f;
	OnCurrentTimeChanged.Broadcast(CurGenerationTime);
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputModeUIOnly;
		PlayerController->SetInputMode(InputModeUIOnly);
	}
}

void AagPlayGameMode::IncreaseGeneration()
{
	CurGeneration += 1;
	OnGenerationChanged.Broadcast(CurGeneration);
}

void AagPlayGameMode::FishWin()
{
	bNowDoingFishProduction = true;
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->UnPossess();
		FInputModeUIOnly InputModeUIOnly;
		PlayerController->SetInputMode(InputModeUIOnly);
	}

	OnGameEndEvent.Broadcast();
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

void AagPlayGameMode::PostCreateManager()
{
}

int32 AagPlayGameMode::GetNewMonsterUID()
{
	++_monsterUID;
	return _monsterUID - 1;
}

void AagPlayGameMode::RegisterMovePoint(EagMonsterMovePointType type, TObjectPtr<class AagMonsterMovePoint> movePoint)
{
	_pointContainer.Add(type, movePoint);
}

void AagPlayGameMode::AddMonsterMovePoint(EagMonsterMovePointType type, const TObjectPtr<AagMonsterMovePoint>& movePoint)
{
	if( movePoint == nullptr ){
		return;
	}
	if( _pointContainer.Contains(type) == true ){
		return;
	}
	_pointContainer.Add(type, movePoint);
}

bool AagPlayGameMode::GetMovePointLocation(EagMonsterMovePointType type, FVector& location)
{
	if( _pointContainer.Contains(type) == false ){
		return false;
	}
	location = _pointContainer[type]->GetActorLocation();
	return true;
}

void AagPlayGameMode::SpawnFish()
{
	const APlayerStart* StartPoint = GetPlayerStartPoint();
	if(StartPoint == nullptr || GetWorld() == nullptr) [[unlikely]]
	{
		return;
	}
	
	if(GetWorld() == nullptr) [[unlikely]]
	{
		return;
	}

	AagFish* FishPawn = GetWorld()->SpawnActorDeferred<AagFish>(DefaultPawnClass, FTransform(), this);
	if(FishPawn)
	{
		CurrentFish = FishPawn;
		GetWorld()->GetFirstPlayerController()->Possess(FishPawn);
		FishPawn->OnFishSpawnProductionEnd.AddUObject(this, &AagPlayGameMode::OnFishSpawnProductionEnd);
		FishPawn->OnFishDeadProductionEnd.AddUObject(this, &AagPlayGameMode::OnFishDeadProductionEnd);
		FActorSpawnParameters SpawnParameters;
		const FTransform SpawnTransform{ StartPoint->GetActorRotation(), StartPoint->GetActorLocation() };
		UGameplayStatics::FinishSpawningActor(CurrentFish, SpawnTransform);
		CurrentFish->PlayFishSpawnProduction();
	}
}

void AagPlayGameMode::OnFishDeadProductionEnd()
{
	SpawnFish();
}

void AagPlayGameMode::OnFishSpawnProductionEnd()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
	}
	
	GenerationStart();
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
				newManagerObject->PostCreated( this );
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
