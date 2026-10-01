// ═══════════════════════════════════════════
//  CollectibleBase.cpp
// ═══════════════════════════════════════════
#include "CollectibleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ExploCharacter.h"
#include "Kismet/GameplayStatics.h"

ACollectibleBase::ACollectibleBase()
{
    PrimaryActorTick.bCanEverTick = false;

    // ─── Root: Overlap Sphere ───
    OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
    RootComponent = OverlapSphere;
    OverlapSphere->SetSphereRadius(100.f);
    OverlapSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    OverlapSphere->SetGenerateOverlapEvents(true);

    // ─── Mesh (visual only, no collision) ───
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // ─── Rotation Animation ───
    RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
    RotatingComp->RotationRate = FRotator(0.f, 90.f, 0.f);

    // Single-player only; no replication needed
    bReplicates = false;
}

void ACollectibleBase::BeginPlay()
{
    Super::BeginPlay();

    // Bind the overlap event
    OverlapSphere->OnComponentBeginOverlap.AddDynamic(
        this, &ACollectibleBase::OnSphereOverlap);
}

// ─── Overlap Event ───
void ACollectibleBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComp,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
    // Ignore if auto-collect is off or already collected
    if (!bAutoCollectOnOverlap || bIsCollected) return;

    if (AExploCharacter* Char = Cast<AExploCharacter>(OtherActor))
    {
        Collect(Char);
    }
}

// ─── Interface: OnInteract ───
void ACollectibleBase::OnInteract_Implementation(AActor* Interactor)
{
    if (bIsCollected) return;

    if (AExploCharacter* Char = Cast<AExploCharacter>(Interactor))
    {
        Collect(Char);
    }
}

// ─── Interface: CanInteract ───
bool ACollectibleBase::CanInteract_Implementation(AActor* Interactor)
{
    return !bIsCollected;
}

// ─── Collection Logic ───
void ACollectibleBase::Collect(AExploCharacter* Collector)
{
    if (bIsCollected || !Collector) return;

    bIsCollected = true;

    // 1. Notify listeners (e.g., score system)
    OnCollected.Broadcast(ItemID, PointValue);

    // 2. Trigger Blueprint VFX
    OnCollectedVFX();

    // 3. Play sound (optional)
    // UGameplayStatics::PlaySoundAtLocation(...)

    // 4. Hide mesh and disable collision (actor lives briefly so VFX can play)
    MeshComp->SetVisibility(false);
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 5. Destroy the actor after 1 second
    SetLifeSpan(1.0f);
}