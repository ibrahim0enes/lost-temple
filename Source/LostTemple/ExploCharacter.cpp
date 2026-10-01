// ═══════════════════════════════════════════
//  ExploCharacter.cpp
// ═══════════════════════════════════════════
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
#include "Interactable.h" 
#include "Chaos/AABBTree.h"
#include "Engine/OverlapResult.h"

AExploCharacter::AExploCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ─── Spring Arm ───
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 1200.f;
    SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    SpringArm->bDoCollisionTest = false;        // Not needed for top-down

    // Yaw follows the controller (via Look); pitch/roll stay fixed
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritPitch = false;
    SpringArm->bInheritRoll = false;

    // ─── Camera ───
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->bUsePawnControlRotation = false;

    // ─── Character Movement ───
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->bOrientRotationToMovement = true;  // Face movement direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

    // ─── Rotation ───
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void AExploCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Apply WalkSpeed in case it was overridden in Blueprint
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Register the Input Mapping Context with the local player
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PC->GetLocalPlayer()))
        {
            if (DefaultContext)
            {
                Subsystem->AddMappingContext(DefaultContext, 0);
            }
        }
    }
}

void AExploCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AExploCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind input actions (Enhanced Input)
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (MoveAction)
        {
            EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExploCharacter::Move);
        }
        if (LookAction)
        {
            EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExploCharacter::Look);
        }
        if (InteractAction)
        {
            EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AExploCharacter::Interact);
        }
    }
}

// ─── Movement (X = right/left, Y = forward/back) ───
void AExploCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MoveInput = Value.Get<FVector2D>();
    if (!Controller || MoveInput.IsNearlyZero()) return;

    // Move relative to camera yaw: W = up on screen
    const FRotator YawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);

    const FVector ForwardDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDir   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDir, MoveInput.Y);
    AddMovementInput(RightDir,   MoveInput.X);
}

// ─── Camera Rotation ───
void AExploCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookInput = Value.Get<FVector2D>();
    AddControllerYawInput(LookInput.X);
}

// ─── Interaction ───
void AExploCharacter::Interact(const FInputActionValue& Value)
{
    const float InteractRadius = 200.f;
    TArray<FOverlapResult> Overlaps;
    
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    
    // Find nearby actors within the interaction radius
    GetWorld()->OverlapMultiByChannel(
        Overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_WorldDynamic,
        FCollisionShape::MakeSphere(InteractRadius),
        Params
    );
    
    // Interact with every valid IInteractable in range
    for (const FOverlapResult& Result : Overlaps)
    {
        AActor* HitActor = Result.GetActor();
        if (!HitActor) continue;

        if (HitActor->Implements<UInteractable>())
        {
            if (IInteractable::Execute_CanInteract(HitActor, this))
            {
                IInteractable::Execute_OnInteract(HitActor, this);
                UE_LOG(LogTemp, Log, TEXT("Interact: %s"), *HitActor->GetName());
            }
        }
    }
    
}