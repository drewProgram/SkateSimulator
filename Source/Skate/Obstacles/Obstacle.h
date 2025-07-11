// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UENUM(BlueprintType)
enum class EObstacleType : uint8
{
	SMALL,
	MEDIUM,
	LARGE,
	END
};

UCLASS()
class SKATE_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	EObstacleType GetObstacleType() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	UStaticMeshComponent* ObstacleMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetObstacleType)
	EObstacleType ObstacleType;

};
