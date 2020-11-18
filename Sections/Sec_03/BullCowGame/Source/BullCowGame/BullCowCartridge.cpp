// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // called when Unreal game starts
{
	Super::BeginPlay();

	// load word list from Content directory into HiddenWords at runtime
	const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
	FFileHelper::LoadFileToStringArrayWithPredicate(HiddenWords, *WordListPath,
		[](const FString& Word) { return IsIsogram(Word); });

	SetUpGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // called when player hits enter
{
	// if game is over, begin new one
	if (bGameOver)
	{
		ClearScreen();
		SetUpGame();
		return;
	}

	// prompt player to set difficulty
	if (!bDifficultySet)
	{
		SetDifficulty(Input);
		return;
	}

	// if guess is invalid, prompt for new guess
	if (!IsValid(Input))
	{
		return;
	}

	// if guess is correct, prompt for new game
	if (Input == HiddenWord)
	{
		PrintLine(TEXT("You guessed the isogram correctly!"));
		EndGame();
		return;
	}
	else
	{
		// guess was wrong; deduct attempt and show results
		--PlayerAttempts;
		PrintResults(Input);
		PrintLine(TEXT("You have %i attempts remaining."), PlayerAttempts);

		// if player is out of attempts, prompt for new game
		if (!PlayerAttempts)
		{
			PrintLine(TEXT("You've run out of lives!"));
			PrintLine(TEXT("The correct isogram was \"%s.\""), *HiddenWord);
			EndGame();
			return;
		}
	}
}

/*
	Checks if word has no repeating characters.
*/
bool UBullCowCartridge::IsIsogram(const FString& Word)
{
	for (int32 WordIndex = 0; WordIndex < Word.Len(); ++WordIndex)
	{
		for (int32 PartialIndex = 0; PartialIndex < WordIndex; ++PartialIndex)
		{
			if (Word[WordIndex] == Word[PartialIndex])
			{
				return false;
			}
		}
	}

	return true;
}

/*
	Checks if guess is isogram of correct length.
*/
bool UBullCowCartridge::IsValid(const FString& Guess) const
{
	if (Guess.Len() != HiddenWord.Len())
	{
		PrintLine(TEXT("Your guess must be exactly %i letters long."), HiddenWord.Len());
		return false;
	}
	else if (!IsIsogram(Guess))
	{
		PrintLine(TEXT("Your guess must be an isogram."));
		return false;
	}

	return true;
}

/*
	Calculates number of bulls and cows and displays results to user.
*/
void UBullCowCartridge::PrintResults(const FString& Guess) const
{
	int32 NumBulls = 0;
	int32 NumCows = 0;

	for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); ++GuessIndex)
	{
		if (Guess[GuessIndex] == HiddenWord[GuessIndex])
		{
			++NumBulls;
			continue;
		}

		for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); ++HiddenIndex)
		{
			if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
			{
				++NumCows;
				break;
			}
		}
	}

	PrintLine(TEXT("Your guess contained %i bulls and %i cows."), NumBulls, NumCows);
}

/*
	(Re)sets member variables in preparation for new round and introduces player to game.
*/
void UBullCowCartridge::SetUpGame()
{
	// set HiddenWord to random isogram from HiddenWords
	int32 RandomIndex = FMath::RandRange(0, HiddenWords.Num() - 1);
	HiddenWord = HiddenWords[RandomIndex];
	PrintLine(TEXT("Hidden word is: %s"), *HiddenWord);
	bDifficultySet = false;
	bGameOver = false;

	PrintLine(TEXT("Welcome to Bulls and Cows!"));
	PrintLine(TEXT("Please select a difficulty."));
	PrintLine(TEXT("Enter %i (Easy), %i (Normal), or %i (Hard)."),
		DifficultyLevels::Easy, DifficultyLevels::Normal, DifficultyLevels::Hard);
}

/*
	Sets difficulty of game and prompts for guesses.
*/
void UBullCowCartridge::SetDifficulty(const FString& Input)
{
	if (!Input.IsEmpty())
	{
		switch (Input[0])
		{
		case '1':
			DifficultyLevel = static_cast<int32>(DifficultyLevels::Hard);
			break;
		case '2':
			DifficultyLevel = static_cast<int32>(DifficultyLevels::Normal);
			break;
		case '3':
			DifficultyLevel = static_cast<int32>(DifficultyLevels::Easy);
			break;
		default:
			PrintLine(TEXT("Please enter a number between %i and %i."),
				DifficultyLevels::Hard, DifficultyLevels::Easy);
			return;
		}
	}
	else
	{
		PrintLine(TEXT("Please enter a number between %i and %i."),
			DifficultyLevels::Hard, DifficultyLevels::Easy);
		return;
	}

	PlayerAttempts = HiddenWord.Len() * DifficultyLevel;
	bDifficultySet = true;

	PrintLine(TEXT("You have set the difficulty to %i."), DifficultyLevel);
	PrintLine(TEXT("Guess the %i-letter isogram in %i attempts."), HiddenWord.Len(), PlayerAttempts);
	PrintLine(TEXT("Press enter to submit your guess."));
	return;
}

/*
	Prepares for new round and prompts player to start a new game.
*/
void UBullCowCartridge::EndGame()
{
	bGameOver = true;
	PrintLine(TEXT("Press enter to play again."));
}