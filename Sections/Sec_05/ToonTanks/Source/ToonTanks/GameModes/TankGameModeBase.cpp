// Fill out your copyright notice in the Description page of Project Settings.

#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor* DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		HandleGameOver(false);
	}
	else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor))
	{
		DestroyedTurret->HandleDestruction();

		if (!(--TargetTurrets))
		{
			HandleGameOver(true);
		}
	}
}

int32 ATankGameModeBase::GetTargetTurretCount()
{
	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(this, APawnTurret::StaticClass(), TurretActors);
	return TurretActors.Num();
}

void ATankGameModeBase::HandleGameStart()
{
	TargetTurrets = GetTargetTurretCount();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GameStart();
	// initialize the start countdown, turret activation, pawn check, etc.
	// call blueprint version of GameStart()
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon)
{
	if (PlayerWon)
	{
		UE_LOG(LogTemp, Error, TEXT("You won."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("You lost."));
	}

	GameOver(PlayerWon);
	// see if player has destroyed all turrets
	// if so, show win result, otherwise, show lose result
	// call blueprint version of GameOver()
}
