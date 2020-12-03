// Copyright Gregory Foster 2020

#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	TargetYaw = GetOwner()->GetActorRotation().Yaw + OpenAngle;

	FindPressurePlate();
	FindDoorSound();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// check if enough mass is overlapping trigger volume
	if (FindTotalOverlappingMass() >= RequiredOverlappingMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpenedTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		// close door if too little mass is overlapping trigger volume
		if (GetWorld()->GetTimeSeconds() - DoorLastOpenedTime >= DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

// Logs error if PressurePlate component cannot be found
void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s's OpenDoor component does not have a PressurePlate set."), *GetOwner()->GetName());
	}
}

// Sets DoorSound component and logs error if one cannot be found
void UOpenDoor::FindDoorSound()
{
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("%s's OpenDoor component does not have a DoorSound component set."), *GetOwner()->GetName());
	}
}

// Calculates total mass of all actors in trigger volume
float UOpenDoor::FindTotalOverlappingMass()
{
	float TotalOverlappingMass = 0.f;

	if (PressurePlate)
	{
		TSet<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		// add up mass of overlapping actors
		for (AActor* OverlappingActor : OverlappingActors)
		{
			TotalOverlappingMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return TotalOverlappingMass;
}

// Opens door to target rotation
void UOpenDoor::OpenDoor(float DeltaTime)
{
	// play sound if door has begun opening
	if (DoorSound && !DoorIsOpening)
	{
		DoorSound->Play();
		DoorIsOpening = true;
	}

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}

// Closes door to initial rotation
void UOpenDoor::CloseDoor(float DeltaTime)
{
	// play sound if door has begun closing
	if (DoorSound && DoorIsOpening)
	{
		DoorSound->Play();
		DoorIsOpening = false;
	}

	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw - OpenAngle, DeltaTime, DoorCloseSpeed);
	GetOwner()->SetActorRotation(CurrentRotation);
}
