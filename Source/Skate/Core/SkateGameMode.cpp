// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateGameMode.h"
#include "SkatePlayerController.h"
#include "../Obstacles/Obstacle.h"
#include "../Character/SkateCharacter.h"
#include "Kismet/GameplayStatics.h"

ASkateGameMode::ASkateGameMode()
	: Score(0),
	DetectedObstacle(nullptr),
	Player(nullptr),
	PlayerController(nullptr),
	MatchTime(120.0f),
	bShouldComputeScore(false)
{
}

bool ASkateGameMode::CheckObstacleWasDetected()
{
	if (DetectedObstacle)
	{
		return true;
	}

	return false;
}

void ASkateGameMode::CheckTouchedDetectedObstacle(AObstacle* Obstacle)
{
	if (DetectedObstacle)
	{
		if (DetectedObstacle == Obstacle)
		{
			bShouldComputeScore = false;
			ClearDetectedObstacle();
		}
	}
}

void ASkateGameMode::SetDetectedObstacle(AObstacle* Obstacle)
{
	UE_LOG(LogTemp, Warning, TEXT("Detected obstacle: %s"), *GetNameSafe(Obstacle));
	DetectedObstacle = Obstacle;
	bShouldComputeScore = true;
}

void ASkateGameMode::ClearDetectedObstacle()
{
	DetectedObstacle = nullptr;
}

int32 ASkateGameMode::GetScore() const
{
	return Score;
}

void ASkateGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ASkateCharacter* SkatePlayer = Cast<ASkateCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player = SkatePlayer;

		if (ASkatePlayerController* PC = Cast<ASkatePlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PlayerController = PC;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerController to ASkatePlayerController in %s"), *GetNameSafe(this));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast Player to ASkateCharacter in %s"), *GetNameSafe(this));
	}

	HandleGameStart();
}

void ASkateGameMode::HandleGameStart()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASkateGameMode::HandleGameOver, MatchTime, false);
}

void ASkateGameMode::HandleGameOver()
{
	PlayerController->SetPlayerEnabledState(false);
	OnMatchEnded.Broadcast();
}

void ASkateGameMode::TryComputeScore()
{
	if (bShouldComputeScore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Computing score..."));
		ComputeScore();
		bShouldComputeScore = false;
	}
}

void ASkateGameMode::ComputeScore()
{
	UE_LOG(LogTemp, Warning, TEXT("Entered ComputeScore()"));
	EObstacleType ObstacleType = DetectedObstacle->GetObstacleType();
	ClearDetectedObstacle();

	switch (ObstacleType)
	{
	case EObstacleType::SMALL:
		Score += 2;
		break;

	case EObstacleType::MEDIUM:
		Score += 4;
		break;

	case EObstacleType::LARGE:
		Score += 8;
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Score updated: %d"), Score);
}
