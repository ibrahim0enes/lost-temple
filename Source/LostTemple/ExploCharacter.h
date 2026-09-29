// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ExploCharacter.generated.h"

UCLASS()
class LOSTTEMPLE_API AExploCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AExploCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_MoveForward;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_MoveRight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_LookUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	class UInputAction* IA_LookRight;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void LookRight(float Value);
	
	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
