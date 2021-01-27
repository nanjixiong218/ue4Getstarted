// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GetCapsuleComponent()->SetCapsuleSize(35.0f, 100.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.15;

	BaseTurnRate = 65.0f;
	BaseLookupRate = 65.0f;

	MaxHealth = 100.0f;
	Health = MaxHealth;
	MaxStamina = 150.0f;
	Stamina = MaxStamina;
	StaminaConsumeRate = 20.0f;
	ExhaustedStaminaRatio = 0.167f;
	StaminaStatus = EPlayerStaminaStatus::EPSS_Normal;
	RunSpeed = 600.0f;
	SprintingSpeed = 900.0f;
	bLeftShiftKeyDown = false;
	Coins = 0;
	MovementStatus = EPlayerMovementStatus::EPMS_Dead;

	
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	switch (StaminaStatus)
	{
	case EPlayerStaminaStatus::EPSS_Normal:
		if (bLeftShiftKeyDown) 
		{
			if (Stamina - StaminaConsumeRate * DeltaTime <= MaxStamina * ExhaustedStaminaRatio)
			{
				StaminaStatus = EPlayerStaminaStatus::EPSS_Exhausted;
			}
			Stamina -= StaminaConsumeRate * DeltaTime;
			SetMovementStatus(EPlayerMovementStatus::EPMS_Sprinting);
		}
		else
		{
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	case EPlayerStaminaStatus::EPSS_Exhausted:
		if (bLeftShiftKeyDown)
		{
			if (Stamina - StaminaConsumeRate * DeltaTime <= 0.0f)
			{
				StaminaStatus = EPlayerStaminaStatus::EPSS_ExhastedRecovering;
				LeftShiftKeyUp();
				SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
			}
			else {
				Stamina -= StaminaConsumeRate * DeltaTime;
			}
		}
		else
		{
			StaminaStatus = EPlayerStaminaStatus::EPSS_ExhastedRecovering;
			Stamina = FMath::Clamp(Stamina + StaminaConsumeRate * DeltaTime, 0.0f, MaxStamina);
			SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		}
		break;
	case EPlayerStaminaStatus::EPSS_ExhastedRecovering:
		if (Stamina + StaminaConsumeRate * DeltaTime > MaxStamina * ExhaustedStaminaRatio) {
			StaminaStatus = EPlayerStaminaStatus::EPSS_Normal;
		}
		Stamina += StaminaConsumeRate * DeltaTime;
		LeftShiftKeyUp();
		SetMovementStatus(EPlayerMovementStatus::EPMS_Normal);
		break;
	default:
		break;
	}

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	// TODO: StopJumping有啥用？
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainPlayer::LeftShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainPlayer::LeftShiftKeyUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayer::LookUp);

	PlayerInputComponent->BindAxis("TurnAtRate", this, &AMainPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &AMainPlayer::LookUpAtRate);



}

void AMainPlayer::Jump()
{
	Super::Jump();
}

void AMainPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		FRotator Rotation = Controller->GetControlRotation();
		// TODO: Rotation中无法直接获取向前的Vector么，必须要自己用矩阵YawRotation加矩阵转换么
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	
	
	/*AddMovementInput(GetActorForwardVector(), Value);*/
}

void AMainPlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
	

	/*AddMovementInput(GetActorRightVector(), Value);*/
}

void AMainPlayer::Turn(float Value)
{
	if (Value != 0.0f) {
		AddControllerYawInput(Value);
	}

}

void AMainPlayer::LookUp(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), Controller->GetControlRotation().Pitch);
	FRotator ControlRotation = Controller->GetControlRotation();
	if (ControlRotation.Pitch < 270.0f && ControlRotation.Pitch > 180.0f && Value > 0.0f) {
	
		return;
	}
	if (ControlRotation.Pitch < 180.0f && ControlRotation.Pitch > 45.0f && Value < 0.0f) {
		return;
	}
	
	AddControllerPitchInput(Value);
}

void AMainPlayer::TurnAtRate(float Rate)
{
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	Turn(Value);
}

void AMainPlayer::LookUpAtRate(float Rate)
{
	
	float Value = Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	LookUp(Value);
}

void AMainPlayer::IncreaseHealth(float Value)
{
	Health = FMath::Clamp(Health + Value, 0.0f, MaxHealth);
}

void AMainPlayer::IncreaseStamina(float Value)
{
	Stamina = FMath::Clamp(Stamina + Value, 0.0f, MaxStamina);
}

void AMainPlayer::IncreaseCoin(int32 Value)
{
	Coins += Value;
}

float AMainPlayer::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - Damage <= 0.0f)
	{
		Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
		// TODO Die()
	}
	else {
		Health -= Damage;
	}
	return Health;
}

void AMainPlayer::SetMovementStatus(EPlayerMovementStatus Status)
{
	MovementStatus = Status;
	switch (MovementStatus) {
	case EPlayerMovementStatus::EPMS_Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		break;
	}
}

