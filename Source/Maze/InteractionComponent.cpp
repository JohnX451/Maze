// Copyright Lukas Fabian, Christian Geßner, Nadia Hofer, Arthur Klammer 2023/2024


#include "InteractionComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Vector.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetUpInputComponent();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Trace, this, &UInteractionComponent::TraceAction, 0.05f, true);
}


void UInteractionComponent::TraceAction()
{
	const auto HitResult = GetFirstWorldDynamicInReach();

	if (const auto HitActor = HitResult.GetActor())
	{
		if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()) && HitActor != CurrentlyHighlightedActor)
		{
			SetHighlight(CurrentlyHighlightedActor, false);
			CurrentlyHighlightedActor = HitActor;
			SetHighlight(CurrentlyHighlightedActor, true);
		}
	}
	else {
		if (!HitActor && CurrentlyHighlightedActor)
		{
			SetHighlight(CurrentlyHighlightedActor, false);
			CurrentlyHighlightedActor = nullptr;
		}
	}
}

void UInteractionComponent::SetHighlight(AActor* Interactee, const bool bIsHighlighted)
{
	if (Interactee) {
		TArray<UActorComponent*> HighlightableComponents = Interactee->GetComponentsByTag(UMeshComponent::StaticClass(), FName("highlightable"));
		if (HighlightableComponents.IsEmpty()) return;
		for (UActorComponent* MeshComp : HighlightableComponents)
		{
			Cast<UMeshComponent>(MeshComp)->SetRenderCustomDepth(bIsHighlighted);
		}
	}
}

void UInteractionComponent::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &UInteractionComponent::Interact);
	}
}

FHitResult UInteractionComponent::GetFirstWorldDynamicInReach()
{
	if (bDebug) DrawDebugLine(GetWorld(), GetReachLineStart(), GetReachLineEnd(Reach), FColor(255,0,0,64), false, 0.2f, 0, 2.0f);

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	// Defining channels which can be traced
	FCollisionObjectQueryParams TraceCollisionChannels;
	TraceCollisionChannels.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetReachLineStart(), GetReachLineEnd(Reach), FCollisionObjectQueryParams(TraceCollisionChannels), TraceParameters);

	return Hit;
}

FVector UInteractionComponent::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UInteractionComponent::GetReachLineEnd(const float Distance) const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Distance;
}

void UInteractionComponent::Interact()
{
	if (CurrentlyHighlightedActor)
	{
		Cast<IInteractable>(CurrentlyHighlightedActor)->Execute_use(CurrentlyHighlightedActor);
	}
}