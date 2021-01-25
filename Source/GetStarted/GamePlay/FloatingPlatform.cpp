// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	StartPoint = FVector(0.0f);
	EndPoint = FVector(0.0f);

	InterpSpeed = 2.0f;

	DelayTime = 2.0f;
	bInterping = true;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	// TODO: 变换世界坐标，没懂
	EndPoint += StartPoint;
	Distance = (EndPoint - StartPoint).Size();
	
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (bInterping) {
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(NewLocation);
		float NowDistance = (GetActorLocation() - StartPoint).Size();
		if (Distance - NowDistance <= 0.5f) {
			bInterping = !bInterping;
			GetWorldTimerManager().SetTimer(InterpTimerHandle, FTimerDelegate::CreateLambda([this]() {
				bInterping = !bInterping;
			}), DelayTime, false);

			FVector TempVec = StartPoint;
			StartPoint = EndPoint;
			EndPoint = TempVec;
		}
	}
}

