#include "AagPlayGameMode.h"
#include "agGameModeExtraData.h"
#include "EngineUtils.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Again30/Monster/agMonsterBase.h"
#include "Again30/Fish/agFish.h"
#include "Again30/Manager/agMonsterMoveManager.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AagPlayGameMode::AagPlayGameMode()
	:
	GenerationTime(10.f), CurGeneration(0), CameraAboveHeight(400.f)
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

	SpectatorCameraActor = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());

	if(CurrentMonster == nullptr)
	{
		CurrentMonster = Cast<AagMonsterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AagMonsterBase::StaticClass()));
	}

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

	GameStart();
}

void AagPlayGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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
	bNowDoingFishProduction = true;
	SpawnFish();
}

void AagPlayGameMode::GenerationStart()
{
	if(bNowDoingFishProduction == false)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("*** *** *** Generation Start"));
	bNowDoingFishProduction = false;
	CurGenerationTime = GenerationTime;
	IncreaseGeneration();
}

void AagPlayGameMode::GenerationEnd()
{
	if(CurrentFish)
	{
		CurrentFish->DisableInput(GetWorld()->GetFirstPlayerController());
		CurrentFish->UnPossessed();
		SetProductionCamera(CurrentFish);
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


void AagPlayGameMode::SetProductionCamera(AagFish* FishPawn)
{
	if(bDisableProduction) [[unlikely]]
	{
		return;
	}
	
	if(FishPawn && SpectatorCameraActor)
	{
		FVector CameraPosition = FishPawn->GetActorLocation();
		CameraPosition.Z += CameraAboveHeight;
		const FRotator CameraRotation = FVector(FishPawn->GetActorLocation() - CameraPosition).Rotation();
		SpectatorCameraActor->SetActorLocation(CameraPosition);
		SpectatorCameraActor->SetActorRotation(CameraRotation);

		if(GetWorld()->GetFirstPlayerController())
		{
			GetWorld()->GetFirstPlayerController()->SetViewTarget(SpectatorCameraActor);
		}
	}
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
		if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PlayerController->Possess(FishPawn);
			FishPawn->DisableInput(PlayerController);
		}
		FishPawn->OnFishSpawnProductionEnd.AddUObject(this, &AagPlayGameMode::OnFishSpawnProductionEnd);
		FishPawn->OnFishDeadProductionEnd.AddUObject(this, &AagPlayGameMode::OnFishDeadProductionEnd);
		FActorSpawnParameters SpawnParameters;
		const FTransform SpawnTransform{ StartPoint->GetActorRotation(), StartPoint->GetActorLocation() };
		UGameplayStatics::FinishSpawningActor(CurrentFish, SpawnTransform);
		CurrentFish->PlayFishSpawnProduction();
	}

	SetProductionCamera(FishPawn);
}

void AagPlayGameMode::OnFishDeadProductionEnd()
{
	SpawnFish();
}

void AagPlayGameMode::OnFishSpawnProductionEnd()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetViewTarget(CurrentFish);
		FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);

		CurrentFish->EnableInput(PlayerController);
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
