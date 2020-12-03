// Copyright Gregory Foster 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetUpInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//SetLineTrace();
	//DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, FColor::Green, false, 0, 100, 10.f);

	// Update location of grabbed object
	if (PhysicsHandleComponent && PhysicsHandleComponent->GrabbedComponent)
	{
		SetLineTrace();
		PhysicsHandleComponent->SetTargetLocation(LineTraceEnd);
	}
}

// Sets PhysicsHandle component and logs error if one cannot be found
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandleComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a PhysicsHandle component set."), *GetOwner()->GetName());
	}
}

// Sets InputComponent and binds "Grab" and "Release" input actions
void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an Input component set."), *GetOwner()->GetName());
	}
}

// Finds intersecting physics body and attaches it to PhysicsHandleComponent
void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBody();
	UPrimitiveComponent* GrabbedComponent = HitResult.GetComponent();

	if (PhysicsHandleComponent && HitResult.GetActor())
	{
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation
		(
			GrabbedComponent,
			NAME_None,
			LineTraceEnd,
			(LineTraceEnd - LineTraceStart / Reach).Rotation()
		);
		PhysicsHandleComponent->GrabbedComponent->SetEnableGravity(false);
		PhysicsHandleComponent->GrabbedComponent->SetLinearDamping(HandleLinearDamping);
	}
}

// Releases currently attached physics body from PhysicsHandleComponent
void UGrabber::Release()
{
	if (PhysicsHandleComponent && PhysicsHandleComponent->GrabbedComponent)
	{
		PhysicsHandleComponent->GrabbedComponent->SetEnableGravity(true);
		PhysicsHandleComponent->GrabbedComponent->SetLinearDamping(0);
		PhysicsHandleComponent->ReleaseComponent();
	}
}

// Line traces from DefaultPawn and returns first intersected physics body
FHitResult UGrabber::GetFirstPhysicsBody()
{
	FHitResult HitObject;
	FCollisionQueryParams TraceParams{ FName{ TEXT("") }, false, GetOwner() };

	SetLineTrace();
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitObject,
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams{ ECollisionChannel::ECC_PhysicsBody },
		TraceParams
	);

	return HitObject;
}

// Updates start and end points of current line trace
void UGrabber::SetLineTrace()
{
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT LineTraceStart, OUT PlayerViewPointRotation);

	LineTraceEnd = LineTraceStart + PlayerViewPointRotation.Vector() * Reach;
}