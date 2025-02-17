// Copyright Lukas Fabian, Christian Geßner, Nadia Hofer, Arthur Klammer 2023/2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MAZE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called for forwards/backwards input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveRight(float Value);
	
	// Called via input to turn at a given rate. 
	void TurnAtRate(float Rate);
	
	// Called via input to turn look up/down at a given rate.  
	void LookUpAtRate(float Rate);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
