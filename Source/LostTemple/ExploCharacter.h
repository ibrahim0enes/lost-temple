// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ExploCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/** Top-down exploration character with camera rotation and interaction. */
UCLASS()
class LOSTTEMPLE_API AExploCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AExploCharacter();

    virtual void Tick(float DeltaTime) override;

    /** Binds Enhanced Input actions. */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    /** Applies movement settings and registers the input mapping context. */
    virtual void BeginPlay() override;

    // ═══ Components ═══

    /** Boom arm holding the top-down camera. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> SpringArm;

    /** Main gameplay camera. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> Camera;

    // ═══ Input Assets ═══

    /** Mapping context added to the local player on BeginPlay. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> DefaultContext;

    /** Movement input (2D axis). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    /** Camera rotation input. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    /** Interaction input (e.g., pick up items). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> InteractAction;

    // ═══ Movement Settings ═══

    /** Maximum walking speed (clamped 100-1000). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",
       meta = (ClampMin = "100", ClampMax = "1000"))
    float WalkSpeed = 400.f;

    // ═══ Input Handlers ═══

    /** Moves relative to camera yaw. */
    void Move(const FInputActionValue& Value);

    /** Rotates the camera (yaw only). */
    void Look(const FInputActionValue& Value);

    /** Interacts with nearby IInteractable actors. */
    void Interact(const FInputActionValue& Value);
};