#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class ASkateCharacter;

UCLASS()
class SKATE_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABaseHUD();

protected:
	virtual void BeginPlay() override;

	ASkateCharacter* PlayerCharacter;

	FTimerHandle NotificationTimer;
	
	void StartTutorialTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowGameOverMessage();

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyTutorial();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleScoreUpdated();


};
