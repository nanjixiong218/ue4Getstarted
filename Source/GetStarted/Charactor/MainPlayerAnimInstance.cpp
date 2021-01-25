// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnimInstance.h"
#include "Charactor/MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// TODO: include 的路径问题

void UMainPlayerAnimInstance::UpdateAnimationProperties()
{
	if (PlayerRef == nullptr)
	{
		PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
	}
	if (PlayerRef)
	{
		FVector SpeedVector = PlayerRef->GetVelocity();
		FVector PlanarSpeed = FVector(SpeedVector.X, SpeedVector.Y, 0.0f);
		Speed = PlanarSpeed.Size();

		bIsInAir = PlayerRef->GetMovementComponent()->IsFalling();
	}
}

void UMainPlayerAnimInstance::NativeInitializeAnimation()
{
	// TODO: Cast 转换
	PlayerRef = Cast<AMainPlayer>(TryGetPawnOwner());
}
