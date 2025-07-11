#include "BaseHUD.h"
#include "../Character/SkateCharacter.h"
#include "../Core/SkateGameMode.h"

#include "Kismet/GameplayStatics.h"

ABaseHUD::ABaseHUD()
	: PlayerCharacter(nullptr)
{
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ASkateCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (PlayerCharacter)
	{
		ASkateGameMode* GameMode = Cast<ASkateGameMode>(UGameplayStatics::GetGameMode(this));
		GameMode->OnScoreUpdated.AddDynamic(this, &ABaseHUD::HandleScoreUpdated);
		GameMode->OnMatchEnded.AddDynamic(this, &ABaseHUD::ShowGameOverMessage);
	}

	StartTutorialTimer();
}

void ABaseHUD::StartTutorialTimer()
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ABaseHUD::DestroyTutorial,
		6.f,
		false
	);
}
