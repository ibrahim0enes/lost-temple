// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/** Interface for actors the player can interact with (pickups, doors, levers). */
class LOSTTEMPLE_API IInteractable
{
	GENERATED_BODY()

public:
	/** Called when an actor interacts with this object. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	void OnInteract(AActor* Interactor);

	/** Returns true if the given actor can currently interact with this object. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
	bool CanInteract(AActor* Interactor);
};