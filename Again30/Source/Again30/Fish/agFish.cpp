﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "agFish.h"

#include "agFishMovementComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Again30/GameMode/AagPlayGameMode.h"
#include "Again30/Weapon/agWeaponBase.h"
#include "Kismet/GameplayStatics.h"

// : Super(ObjectInitializer.SetDefaultSubobjectClass<UagFishMovementComponent>(ACharacter::CharacterMovementComponentName))


AagFish::AagFish(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UagFishMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}


void AagFish::UnPossessed()
{
	Super::UnPossessed();

	if (InputComponent)
	{
		InputComponent->ClearActionBindings();
	}
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent != nullptr)
	{
		MovementComponent->ResetMoveState();
		MovementComponent->StopMovementImmediately();
	}
}

void AagFish::MultiplyHopForce(float Factor)
{
	HopForce *= Factor;
}

void AagFish::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AagFish::SetReadyToHop, HopRate, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AagFish::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// // Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AagFish::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AagFish::JumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AagFish::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AagFish::Look);

		// Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AagFish::Attack);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AagFish::Interact);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AagFish::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentJumpHoldTime += DeltaSeconds;
	
}

void AagFish::SetReadyToHop()
{
	bReadyToHop = true;
}

void AagFish::PlayFishSpawnProduction()
{
	if(GetMesh())
	{
		GetMesh()->bPauseAnims = true;
	}

	if(GetWorld())
	{
		FTimerDelegate TimerENdDelegate;
		TimerENdDelegate.BindUObject(this, &AagFish::OnSpawnProductionEnd);
		GetWorld()->GetTimerManager().SetTimer(TestTimerHandler, TimerENdDelegate, 2.f, false);
	}
}

void AagFish::PlayFishDeadProduction()
{
	if(GetMesh())
	{
		GetMesh()->bPauseAnims = true;
	}

	K2_OnFishDead();

	if(GetWorld())
	{
		FTimerDelegate TimerENdDelegate;
		TimerENdDelegate.BindUObject(this, &AagFish::OnDeadProductionEnd);
		GetWorld()->GetTimerManager().SetTimer(TestTimerHandler, TimerENdDelegate, 2.f, false);
	}
}

void AagFish::OnSpawnProductionEnd()
{
	if(GetMesh())
	{
		GetMesh()->bPauseAnims = false;
	}
	OnFishSpawnProductionEnd.Broadcast();
}

void AagFish::OnDeadProductionEnd()
{
	OnFishDeadProductionEnd.Broadcast();
}

void AagFish::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		if (bReadyToHop)
		{
			FVector Impulse = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;
			Impulse.Normalize();
			
			Impulse += FVector(0.f, 0.f, HopHeight);
			if (!GetCharacterMovement()->IsFalling())
			{
				LaunchCharacter(Impulse * HopForce, true, true);
			}
			bReadyToHop = false;
		}
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AagFish::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AagFish::Attack()
{
	if (Weapon)
	{
		Weapon->DoAttack();
	}
}

void AagFish::Interact()
{
	if (Weapon)
	{
		Weapon->RemoveWeapon();
		Weapon = nullptr;
		return;
	}
	
	if (UWorld* World = GetWorld())
	{
		TArray<FOverlapResult> Overlaps;
		const bool bIsOverlaped = World->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat::Identity,
			FCollisionObjectQueryParams(ag_ObjectChannel_Weapon), FCollisionShape::MakeSphere(200.f));

		TArray<AActor*> OverlappedActors;
		if (bIsOverlaped)
		{
			for (FOverlapResult& Overlap : Overlaps)
			{
				if (AActor* TargetActor = Overlap.GetActor())
				{
					OverlappedActors.AddUnique(TargetActor);
				}
			}
		}
		float Distance;
		AActor* NearestActor = UGameplayStatics::FindNearestActor(GetActorLocation(), OverlappedActors, Distance);
		
		AagWeaponBase* NearestWeapon = Cast<AagWeaponBase>(NearestActor);
		if (NearestWeapon)
		{
			Weapon = NearestWeapon;
			NearestWeapon->EquipWeapon(GetMesh(), FName(TEXT("Socket_Mouse")), this);
		}
	}
}

void AagFish::Jump()
{
	Super::Jump();
	bIsJumping = true;
}

void AagFish::JumpStart()
{
	CurrentJumpHoldTime = 0.0f;
	K2_OnJumpStart();
}

void AagFish::JumpEnd()
{
	GetCharacterMovement()->JumpZVelocity = FMath::Lerp(MinJumpHeight, MaxJumpHeight, FMath::Clamp(CurrentJumpHoldTime / MaxJumpHoldTime, 0.0f, 1.0f));

	if (!bIsJumping)
	{
		Jump();
	}
	K2_OnJumpEnd();
}

