// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkateCharacter.h"
#include "../Core/SkateGameMode.h"
#include "../Obstacles/Obstacle.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASkateCharacter::ASkateCharacter()
{
	GameMode = nullptr;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	TurnSpeed = 45.f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->FallingLateralFriction = 5.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->MaxAcceleration = 260.f;
	//GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 150.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 200.0f;
	GetCharacterMovement()->BrakingFriction = 1.f;
	GetCharacterMovement()->GroundFriction = 1.0f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
	GetCharacterMovement()->bMaintainHorizontalGroundVelocity = false;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 420.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	SkateboardMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkateboardMesh"));
	SkateboardMesh->SetupAttachment(GetMesh(), FName("foot_l"));
	SkateboardMesh->SetRelativeLocation(FVector(-29.14f, -2.66f, -13.52f));
	SkateboardMesh->SetRelativeRotation(FRotator(-184.75f, 56.15f, 84.72f));

	bIsJumping = false;
}

bool ASkateCharacter::GetIsJumping() const
{
	return bIsJumping;
}

void ASkateCharacter::HandleEndJump()
{
	bIsJumping = false;
	OnJumpEnd.ExecuteIfBound();
}

void ASkateCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogTemplateCharacter, Warning, TEXT("Checking if jumping over an object..."));
		if (!GameMode->CheckObstacleWasDetected())
		{
			UE_LOG(LogTemplateCharacter, Warning, TEXT("CheckObstacleWasDetected() false"));
			CheckIfJumpingOverObject();
		}
	}
}

void ASkateCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ASkateGameMode* GM = Cast<ASkateGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode = GM;
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to cast GameMode to ASkateGameMode in %s"), *GetNameSafe(this));
	}

}

void ASkateCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (Hit.GetActor())
	{
		if (AObstacle* Obstacle = Cast<AObstacle>(Hit.GetActor()))
		{
			GameMode->CheckTouchedDetectedObstacle(Obstacle);
			return;
		}
	}

	UE_LOG(LogTemplateCharacter, Warning, TEXT("Landed!!!"));

	GameMode->TryComputeScore();
}

void ASkateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateCharacter::PlayJumpMontage);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateCharacter::Move);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASkateCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);

		AddControllerYawInput(MovementVector.X * TurnSpeed * GetWorld()->GetDeltaSeconds());
	}
}

void ASkateCharacter::SlowDown()
{

}

void ASkateCharacter::PlayJumpMontage()
{
	if (JumpMontage && !bIsJumping)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			bIsJumping = true;
			AnimInstance->Montage_Play(JumpMontage);
		}
	}
}

void ASkateCharacter::CheckIfJumpingOverObject()
{
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, -300.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		TraceParams
	);

	if (bHit && Hit.GetActor())
	{
		if (AObstacle* Obstacle = Cast<AObstacle>(Hit.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Jumping over: %s"), *Hit.GetActor()->GetName());
			GameMode->SetDetectedObstacle(Obstacle);
		}
	}

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);
}

