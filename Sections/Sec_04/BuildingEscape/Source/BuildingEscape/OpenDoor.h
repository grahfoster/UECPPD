// Copyright Gregory Foster 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 5.f;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5f;
	UPROPERTY(EditAnywhere)
	float RequiredOverlappingMass = 50.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
	UAudioComponent* DoorSound = nullptr;

	float TargetYaw;
	float DoorLastOpenedTime = 0.f;
	bool DoorIsOpening = false;

	void FindPressurePlate();
	void FindDoorSound();
	float FindTotalOverlappingMass();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
