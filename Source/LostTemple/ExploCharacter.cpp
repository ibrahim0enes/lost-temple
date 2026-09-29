// Fill out your copyright notice in the Description page of Project Settings.

#include "ExploCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "InputMappingContext.h"

AExploCharacter::AExploCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ─── Spring Arm ───
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 1200.f;
    SpringArm->SetUsingAbsoluteRotation(true);
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritYaw = false;
    SpringArm->bInheritRoll = false;

    // ─── Camera ───
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    // ─── Character Movement ───
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

    // ─── Rotation ───
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void AExploCharacter::BeginPlay()
{
    Super::BeginPlay();

    
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultContext)
            {
                Subsystem->AddMappingContext(DefaultContext, 0);
            }
        }
    }
}

void AExploCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExploCharacter::Move);
        }
        if (InteractAction)
        {
            EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AExploCharacter::Interact);
        }
    }
}

void AExploCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AExploCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveInput = Value.Get<FVector2D>();
    if (!Controller || MoveInput.IsNearlyZero()) return;

    // Yönü kameranın yaw'ına göre hesapla (W = ekranda yukarı)
    const FRotator YawRot(0.f, SpringArm->GetComponentRotation().Yaw, 0.f);

    const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDir, MoveInput.Y);
    AddMovementInput(RightDir,   MoveInput.X);
}

void AExploCharacter::Interact(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Etkileşim tuşuna basıldı! (Aşama 2'de doldurulacak)"));
}