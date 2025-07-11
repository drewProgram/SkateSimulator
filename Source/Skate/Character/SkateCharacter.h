#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/SkeletalMeshComponent.h"
#include "SkateCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ASkateGameMode;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DELEGATE(FOnJumpEndDelegate);

UCLASS(config=Game)
class ASkateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASkateCharacter();
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	bool GetIsJumping() const;

	UFUNCTION(BlueprintCallable)
	void HandleEndJump();

	virtual void Tick(float DeltaSeconds) override;

	FOnJumpEndDelegate OnJumpEnd;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay();

	virtual void Landed(const FHitResult& Hit) override;

	void Move(const FInputActionValue& Value);

	void SlowDown();

	void PlayJumpMontage();

	void CheckIfJumpingOverObject();

private:
	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetIsJumping, Category = "Movement")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkateboardMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* JumpMontage;

	ASkateGameMode* GameMode;
};

