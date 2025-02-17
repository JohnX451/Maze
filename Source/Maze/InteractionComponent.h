// Copyright Lukas Fabian, Christian Geßner, Nadia Hofer, Arthur Klammer 2023/2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UInputComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAZE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Maximum distance from screen position in cm to interact with objects
	UPROPERTY(EditAnywhere)
	float Reach = 180.0f;

	// Actor that is currently being recognized as interactive
	UPROPERTY(EditAnywhere)
	AActor* CurrentlyHighlightedActor;

	// Mesh component that was last highlighted
	UPROPERTY()
	UMeshComponent* LastHighlightedComponent;

	// Timer handle used to continuously trace for actors to highlight
	FTimerHandle TimerHandle_Trace;

	// Show debug trace and log messages
	UPROPERTY(EditAnywhere)
	bool bDebug = false;

	void TraceAction();

	static void SetHighlight(AActor* Interactee, bool bIsHighlighted);
	
public:	
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

private:
	// Set up assumed attached input component
	void SetUpInputComponent();

	// Return hit for first physics body in reach
	FHitResult GetFirstWorldDynamicInReach();

	// Returns current start of reach line
	FVector GetReachLineStart() const;

	// Returns current end of reach line
	FVector GetReachLineEnd(float Distance) const;

	void Interact();
};
