// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "CollectibleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class URotatingMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCollectedSignature, FName, ItemID, int32, Value
);

UCLASS()
class LOSTTEMPLE_API ACollectibleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectibleBase();
	
	UPROPERTY(BlueprintAssignable, Category = "Collectible")
	FOnCollectedSignature OnCollected;
	
	virtual void OnInteract_Implementation(AActor* Interactor) override;
	virtual bool CanInteract_Implementation(AActor* Interactor) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* OverlapSphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URotatingMovementComponent* RotatingComp;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp,
						 AActor* OtherActor,
						 UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex,
						 bool bFromSweep,
						 const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID = TEXT("Coin");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bAutoCollectOnOverlap = true;

	bool bIsCollected = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void OnCollectedVFX();

	void Collect(class AExploCharacter* Collector);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
