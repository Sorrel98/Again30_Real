// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "agFish.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_EVENT(AagFish, FFishEvent);

UCLASS()
class AGAIN30_API AagFish : public ACharacter
{
	GENERATED_BODY()

public:
	FFishEvent OnFishSpawnProductionEnd;
	FFishEvent OnFishDeadProductionEnd;
	
private:
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	


	/** Jump Property */

	UPROPERTY(EditAnywhere, Category = "Jump")
	float MaxJumpHoldTime;

	float CurrentJumpHoldTime;
	
	UPROPERTY(EditAnywhere, Category = "Jump")
	float MaxJumpHeight;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float MinJumpHeight;

	/** Hopping Property */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float HopHeight;

	UPROPERTY(EditAnywhere)
	float HopForce;

	UPROPERTY(EditAnywhere)
	float HopRate;

	bool bReadyToHop;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

public:
	// Sets default values for this character's properties
	AagFish(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UnPossessed() override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	void SetReadyToHop();
	/** 연출이 있기 전 실험용 타이머로 망할 연출을 제어하도록 하마 (나는 연출의 마법사가 될꺼야) */
	FTimerHandle TestTimerHandler;
	void PlayFishSpawnProduction();
	void PlayFishDeadProduction();
	void OnSpawnProductionEnd();
	void OnDeadProductionEnd();

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnFishDead", Category = "Fish")
	void K2_OnFishDead();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Attack();

	void Interact();

	virtual void Jump() override;

	void JumpStart();

	UFUNCTION(BlueprintImplementableEvent, Category = "Jump", DisplayName = "OnJumpStart")
	void K2_OnJumpStart();

	void JumpEnd();

	UFUNCTION(BlueprintImplementableEvent, Category = "Jump", DisplayName = "OnJumpEnd")
	void K2_OnJumpEnd();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
