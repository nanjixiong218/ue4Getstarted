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

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::Jump);
	// TODO: StopJumping有啥用？
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

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

