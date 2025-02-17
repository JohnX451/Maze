// Copyright Lukas Fabian, Christian Geßner, Nadia Hofer, Arthur Klammer 2023/2024

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

/**
 * Usage: the actor implementing this interface must have a static mesh component with the tag "highlightable"
 */
class MAZE_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void use();
};
