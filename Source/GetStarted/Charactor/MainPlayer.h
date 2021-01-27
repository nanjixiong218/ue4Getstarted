// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementStatus :uint8 {
	EPMS_Normal UMETA(DisplayName="Normal"),
	EPMS_Sprinting UMETA(DisplayName = "Normal"),
	EPMS_Dead UMETA(DisplayName = "Normal"),
};
UENUM(BlueprintType)
enum class EPlayerStaminaStatus :uint8 {
	EPSS_Normal UMETA(DisplayName = "Normal"),
	EPSS_Exhausted UMETA(DisplayName = "Exhausted"),
	EPSS_ExhastedRecovering UMETA(DisplayName = "ExhastedRecovering"),
};

UCLASS()
class GETSTARTED_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="basic")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "basic")
	float BaseLookupRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float StaminaConsumeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats", meta = (ClampMin = 0, ClampMax = 1, UIMin = 0, UIMax = 1))
	float ExhaustedStaminaRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerStaminaStatus StaminaStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float SprintingSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	EPlayerMovementStatus MovementStatus;

	bool bLeftShiftKeyDown;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// TODO: 基类里有Jump, 这里为什么还要写一遍，然后再实现，直接在cpp里实现不行么
	virtual void Jump() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);
	
	void LookUp(float Value);

	void TurnAtRate(float Rate);
	
	void LookUpAtRate(float Rate);
	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float Value);
	UFUNCTION(BlueprintCallable)
	void IncreaseStamina(float Value);
	UFUNCTION(BlueprintCallable)
	void IncreaseCoin(int32 Value);
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE void LeftShiftKeyDown() { bLeftShiftKeyDown = true; };
	FORCEINLINE void LeftShiftKeyUp() { bLeftShiftKeyDown = false; };

	void SetMovementStatus(EPlayerMovementStatus Status);

};
