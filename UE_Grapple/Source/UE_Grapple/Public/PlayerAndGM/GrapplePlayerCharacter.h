#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GrapplePlayerCharacter.generated.h"

//UInputAction;

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AGrappleShooter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSingleEvent);

UCLASS()
class UE_GRAPPLE_API AGrapplePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrapplePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//Components_____
public:
	UCameraComponent* Camera;

//Input_____
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ShootGrapplingHookAction;


//Movement_____
protected:

	//looking
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float TurningSpeed=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float PitchLimit = 70;

	//Jumping
	void StartJump();
	void EndJump();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSingleEvent OnStartJump;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSingleEvent OnEndJump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bJumpButtonDown=false;

protected:
	//walking
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float WalkingSpeed = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float SprintingSpeed = 600;

	void StartSprinting();
	void StopSprinting();
	

//Grappling_____
protected:
	void ShootGrapplePressed();
	void ShootGrappleEnd();
	
	UPROPERTY(EditDefaultsOnly)
	UChildActorComponent* GrappleShooterChildActor;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGrappleShooter* MyGrappleShooter;

	
//Debug
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug");
	TSubclassOf<UUserWidget> DebugStats;

	//Other
	void SetDefaultValues();


	
	
};
