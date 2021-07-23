// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    //int32 Number = 9;
    
    //PrintLine(TEXT("Bulls = %i Cows = %i "), Count.Bulls, Count.Cows);

    Isograms = GetValidWordsList(Words); // did mistake to put after SetupGame() and we dont need to call every time just once on the first play
    SetupGame(); //Setting up the Game
    
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    //if game is over then do Clearsscreen() and re SetupGame()
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }

    //else keep checking the guess
    else
    {
        //move this whole block in ProcessGuess()
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    //Welcome message for players
    PrintLine(TEXT("Hi there! Welcome to Bull Cow Game..."));
    
    //HiddenWord = GetValidWordsList(Words)[(FMath::RandRange(0, GetValidWordsList(Words).Num() - 1))];
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
    NumberOfLives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess a %i letter word"), HiddenWord.Len());
    
    PrintLine(TEXT("Hidden Word is: %s"), *HiddenWord); //Debug Line

    PrintLine(TEXT("Currently you have %i Lives left"), HiddenWord.Len());
    PrintLine(TEXT("Type your Guess. \nPress Enter to Continue... "));

   
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress Enter to Play Again! "));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Your Guess is correct, You Won! "));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), NumberOfLives);
        return;
    }

    //check if the word is an Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("repeating letters, guess again!"));
        return;
    }

    //remove life if wrong guess
    PrintLine(TEXT("Lost a life!"));
    --NumberOfLives;

    //check if lives>0
    if (NumberOfLives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The Hidden Word was: %s "), *HiddenWord);

        EndGame();
        return;
    }

    //show the player Bulls and Cows
     FBullCowCount Score = GetBullCows(Guess);
    
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);  

    PrintLine(TEXT("Guess again! You have %i lives left"), NumberOfLives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
    //check each char is a diff letter in the whole word

    for (int32 Index = 0; Index < Word.Len() - 1; Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWordsList(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString eachWord : WordList)
    {
        if (eachWord.Len() >= 4 && eachWord.Len() <= 8)
        {
            if (IsIsogram(eachWord))
            {
                ValidWords.Emplace(eachWord);
            }
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    // for every, index Guess == index Hidden then, BullCount++
    // if not a bull was it a cow? if yes then CowCount 

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
        
    }
    return Count;
}
