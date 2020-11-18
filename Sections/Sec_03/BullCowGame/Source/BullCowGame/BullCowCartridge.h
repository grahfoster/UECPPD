// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
private:
	enum class DifficultyLevels { Hard = 1, Normal = 2, Easy = 3 };

	TArray<FString> HiddenWords;
	FString HiddenWord;
	bool bDifficultySet;
	int32 DifficultyLevel;
	int32 PlayerAttempts;
	bool bGameOver;

	static bool IsIsogram(const FString& Word);

	bool IsValid(const FString& Guess) const;
	void PrintResults(const FString& Guess) const;

	void SetUpGame();
	void SetDifficulty(const FString& Guess);
	void EndGame();
};