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

// ─── Delegate: Bir eşya toplandığında fırlatılır ───
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnCollectedSignature, FName, ItemID, int32, Value
);

UCLASS()
class LOSTTEMPLE_API ACollectibleBase : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ACollectibleBase();

    // ═══ Delegate (Aşama 5'te bağlanacak) ═══
    UPROPERTY(BlueprintAssignable, Category = "Collectible")
    FOnCollectedSignature OnCollected;

    // ═══ Interface Implementations ═══
    virtual void OnInteract_Implementation(AActor* Interactor) override;
    virtual bool CanInteract_Implementation(AActor* Interactor) override;

protected:
    virtual void BeginPlay() override;

    // ─── Components ───
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* OverlapSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    URotatingMovementComponent* RotatingComp;

    // ─── Overlap Events ───
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComp,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);

    // ─── Data (DataTable'dan gelecek, Aşama 4) ───
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID = TEXT("Coin");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 PointValue = 10;

    // ─── Auto Collect (üstüne basınca otomatik topla) ───
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bAutoCollectOnOverlap = true;

    // ─── Internal ───
    bool bIsCollected = false;

    UFUNCTION(BlueprintImplementableEvent, Category = "Item")
    void OnCollectedVFX();    // Blueprint'te parçacık/animasyon

    void Collect(class AExploCharacter* Collector);
};