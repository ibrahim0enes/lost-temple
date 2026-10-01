// ═══════════════════════════════════════════
//  CollectibleBase.h
// ═══════════════════════════════════════════
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "CollectibleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;
class AExploCharacter;

// ─── Delegate: broadcast when an item is collected ───
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnCollectedSignature, FName, ItemID, int32, Value
);

/** Base class for pickups (coins, relics, keys). Supports auto and manual collection. */
UCLASS()
class LOSTTEMPLE_API ACollectibleBase : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ACollectibleBase();

    // ═══ Delegate (bound in Stage 5) ═══

    /** Fired after a successful pickup. */
    UPROPERTY(BlueprintAssignable, Category = "Collectible")
    FOnCollectedSignature OnCollected;

    // ═══ Interface Implementations ═══

    /** Manual pickup entry point. */
    virtual void OnInteract_Implementation(AActor* Interactor) override;

    /** Returns false once the item has been collected. */
    virtual bool CanInteract_Implementation(AActor* Interactor) override;

protected:
    /** Binds the overlap event. */
    virtual void BeginPlay() override;

    // ─── Components ───

    /** Pickup range detection. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* OverlapSphere;

    /** Visual mesh. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    /** Spins the mesh for visibility. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    URotatingMovementComponent* RotatingComp;

    // ─── Overlap Events ───

    /** Collects the item on overlap if auto-collect is enabled. */
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComp,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);

    // ─── Data (DataTable-driven in Stage 4) ───

    /** Item type identifier. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID = TEXT("Coin");

    /** Points awarded on pickup. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 PointValue = 10;

    // ─── Auto Collect ───

    /** Collect on overlap instead of requiring interaction. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bAutoCollectOnOverlap = true;

    // ─── Internal ───

    /** Prevents double collection. */
    bool bIsCollected = false;

    /** Blueprint hook for pickup effects (VFX/SFX). */
    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnCollectedVFX();

    /** Marks collected, plays VFX, broadcasts, and destroys the actor. */
    void Collect(AExploCharacter* Collector);
};