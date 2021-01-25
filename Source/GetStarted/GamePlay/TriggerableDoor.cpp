// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerableDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Charactor/MainPlayer.h"
#include "TimerManager.h"

// Sets default values
ATriggerableDoor::ATriggerableDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetBoxExtent(FVector(80.0f, 80.0f, 30.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(GetRootComponent());

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(GetRootComponent());

	DelayTime = 1.5f;
	bIsPlayerOnTrigger = false;
}

// Called when the game starts or when spawned
void ATriggerableDoor::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this,&ATriggerableDoor::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerableDoor::OnOverlapEnd);

	InitTriggerLocation = TriggerMesh->GetComponentLocation();
	InitDoorLocation = DoorMesh->GetComponentLocation();
	
}

// Called every frame
void ATriggerableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerableDoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		bIsPlayerOnTrigger = true;
		//GetWorldTimerManager().ClearTimer(CloseDoorTimerHandle);
		OpenDoor();
		LowerTrigger();
	}
}

void ATriggerableDoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainPlayer* MainPlayer = Cast<AMainPlayer>(OtherActor);
	if (MainPlayer)
	{
		bIsPlayerOnTrigger = false;
		RaiseTrigger();

		auto DelayCloseDoor = [this]() {
			if (!bIsPlayerOnTrigger) {
				CloseDoor();
			}
		};
		//GetWorldTimerManager().SetTimer(CloseDoorTimerHandle, this, &ATriggerableDoor::CloseDoor, DelayTime);
		GetWorldTimerManager().SetTimer(CloseDoorTimerHandle, FTimerDelegate::CreateLambda([this]() {
			if (!bIsPlayerOnTrigger) {
				CloseDoor();
			}
		}), DelayTime, false);
	}
}

void ATriggerableDoor::UpdateTriggerLocation(FVector Offset)
{
	FVector NewLocation = InitTriggerLocation + Offset;
	TriggerMesh->SetWorldLocation(NewLocation);
}

void ATriggerableDoor::UpdateDoorLocation(FVector Offset)
{
	FVector NewLocation = InitDoorLocation + Offset;
	DoorMesh->SetWorldLocation(NewLocation);
}

