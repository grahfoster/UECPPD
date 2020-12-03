// Copyright Gregory Foster 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;
	UPROPERTY(EditAnywhere)
	UInputComponent* InputComponent = nullptr;
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;	// distance from which player can grab objects
	UPROPERTY(EditAnywhere)
	float HandleLinearDamping = 2.5f;


	FVector LineTraceStart;	// = DefaultPawn's origin location
	FVector LineTraceEnd;

	void FindPhysicsHandle();
	void SetUpInputComponent();
	void Grab();
	void Release();
	FHitResult GetFirstPhysicsBody();
	void SetLineTrace();
};
