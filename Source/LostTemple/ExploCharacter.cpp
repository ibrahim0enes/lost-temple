// Fill out your copyright notice in the Description page of Project Settings.

#include "ExploCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


// Sets default values
AExploCharacter::AExploCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ─── Spring Arm ───
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1200.f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	
	// ─── Camera ───
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// ─── Character Movement ───
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	// ─── Rotation ───
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void AExploCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<PlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultContext, 0);
		}
	}
	
}
// Called to bind functionality to input

void AExploCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC =  CastChecked<UENhancedInputComponent>(PlayerInputComponent))
	{
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExploCharacter::Move);
        EIC->BindAction(LookAction,     ETriggerEvent::Triggered, this, &AExploCharacter::Look);
		EIC->BindAction(InteractAction, ETriggerEvent::Started,   this, &AExploCharacter::Interact);
	}
}

// Called every frame
void AExploCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExploCharacter::Move(const FInputActionValue& Value)
{
	FVector2d MoveInput =  Value.Get<FVector2D>();
	if (!Controller ||  MoveInput.IsNearlyZero()) return;
	
	const FRotator CanRot = Controller->GetControlRotation();
    const FRotator YawRot(0.f, CamRot.Yaw, 0.f);
	
	const FVector ForwardDir =  FRotationMatrix(YawRot).GetUnitAxes(EAxis::X);
	const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDir, MoveInput.Y);
	AddMovementInput(RightDir,   MoveInput.X);

}

void AExploCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X);

}

void AExploCharacter::Interact(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Etkileşim tuşuna basıldı! (Aşama 2'de doldurulacak)"));

}

