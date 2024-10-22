

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GrapplePC.generated.h"

/**
 * 
 */
UCLASS()
class UE_GRAPPLE_API AGrapplePC : public APlayerController
{
	GENERATED_BODY()

protected:
	void OnAnyKeyPressed(const FKey& Key);

	void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUsingGamepad;
};
