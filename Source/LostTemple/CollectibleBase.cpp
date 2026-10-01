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

    // ─── Mesh ───
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // ─── Dönme Animasyonu ───
    RotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
    RotatingComp->RotationRate = FRotator(0.f, 90.f, 0.f);

    // ─── Blueprint'ten türetilebilir olduğunu belirt ───
    bReplicates = false;
}

void ACollectibleBase::BeginPlay()
{
    Super::BeginPlay();

    // Overlap event'ini bağla
    OverlapSphere->OnComponentBeginOverlap.AddDynamic(
        this, &ACollectibleBase::OnSphereOverlap);
}

// ─── Overlap Olayı ───
void ACollectibleBase::OnSphereOverlap(UPrimitiveComponent* OverlappedComp,
                                        AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32 OtherBodyIndex,
                                        bool bFromSweep,
                                        const FHitResult& SweepResult)
{
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

// ─── Asıl Toplama ───
void ACollectibleBase::Collect(AExploCharacter* Collector)
{
    if (bIsCollected || !Collector) return;

    bIsCollected = true;

    // 1. Delegate'i tetikle → Score sistemi dinliyor
    OnCollected.Broadcast(ItemID, PointValue);

    // 2. Blueprint VFX'i çalıştır
    OnCollectedVFX();

    // 3. Ses çal (opsiyonel)
    // UGameplayStatics::PlaySoundAtLocation(...)

    // 4. Mesh'i gizle (VFX oynasın diye bir süre bekletilebilir)
    MeshComp->SetVisibility(false);
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 5. 1 saniye sonra Actor'ü yok et
    SetLifeSpan(1.0f);
}