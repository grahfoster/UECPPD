// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleCollider;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	FVector LookAtTargetClean = FVector{ LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z };
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector{ LookAtTargetClean - StartLocation }.Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);
	
	// Update TurretMesh rotation to face the player
}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint location and rotation
	// Spawn projectile class at location facing firing direction

	if (ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}
}

void APawnBase::HandleDestruction()
{
	// Player death effects (particles, sound, camera shake)

	// For PawnTurret, inform GameMode turret died, then destroy
	// For PawnTank, inform GameMode player died, then hide components and disable input
}

