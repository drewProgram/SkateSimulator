// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkatePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKATE_API ASkatePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool bPlayerEnabled);
};
