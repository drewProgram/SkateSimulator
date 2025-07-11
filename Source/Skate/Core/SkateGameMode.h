// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SkateGameMode.generated.h"

class AObstacle;
class ASkateCharacter;
class ASkatePlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchEndedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreUpdated);

UCLASS(minimalapi)
class ASkateGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASkateGameMode();

	bool CheckObstacleWasDetected();
	void CheckTouchedDetectedObstacle(AObstacle* Obstacle);
	void SetDetectedObstacle(AObstacle* Obstacle);
	void ClearDetectedObstacle();

	UFUNCTION(BlueprintCallable)
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable)
	float GetMatchTime() const;

	void TryComputeScore();

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnMatchEndedDelegate OnMatchEnded;

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnScoreUpdated OnScoreUpdated;

protected:
	virtual void BeginPlay() override;

	void HandleGameStart();

	UFUNCTION()
	void HandleGameOver();

	void ComputeScore();

	void RestartGame();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetMatchTime, Category = "Game Rules")
	float MatchTime;

	ASkateCharacter* Player;
	ASkatePlayerController* PlayerController;

	AObstacle* DetectedObstacle;

	bool bShouldComputeScore;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetScore, Category = "Game Rules", meta = (AllowPrivateAccess = "true"))
	int32 Score;
};



