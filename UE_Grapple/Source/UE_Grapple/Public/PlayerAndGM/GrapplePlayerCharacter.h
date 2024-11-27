#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GrapplePlayerCharacter.generated.h"

//UInputAction;

class UGrappleMovementComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AGrappleShooter;

struct FMovementDirecions
{
	bool bForward = false;
	bool bBackward = false;
	bool bRight = false;
	bool bLeft = false;
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSingleEvent);


UCLASS()
class UE_GRAPPLE_API AGrapplePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrapplePlayerCharacter(const FObjectInitializer& ObjectInitializer);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

//Input_____
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* DefaultMappingContext_MaK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* DefaultMappingContext_GamePad;
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	class UGrappleMovementComponent* CharacterMovementComponent;
	FMovementDirecions CurrentMovementDirection;
	
	//looking
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement/Looking")
	float TurningSpeed=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement/Looking")
	float PitchLimit = 70;

	//Jumping
	void JumpButtonDown();
	void EndJump();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSingleEvent OnStartJump;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSingleEvent OnEndJump;

	//for animation blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bJumping=false;

protected:
	//walking
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement/Walking")
	float WalkingSpeed = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement/Walking")
	float SprintingSpeed = 600;

private:
	bool bIsSprinting=false;
	void SprintButtonDown();
	void SprintButtonUp();
	void StartSprinting();
	void StopSprinting();

	//Boosting

protected:
	//Amount of seconds where boosting is possible
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement/Boosting")
	float MaxBoostingFuel=1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement/Boosting")
	float BoostingStrenght=15000;

	//How fast boosting refuels per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement/Boosting")
	float BoostingRefuelSpeed=0.3;

	//How long until boost starts refueling
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement/Boosting")
	float BoostingRefuelDelay=2;
	
private:
	bool bIsBoosting=false;
	float BoostingFuel=0;
	bool bRefuelBoostAllowed=true;
	

	void StartBoosting();
	void StopBoosting();
	void Tick_ApplyBoost();
	void Tick_RefuelBoost();

	void AllowBoostRefill();
	
	FTimerHandle BoostRefuelTimer;

	//WallRunning
private:
	void Tick_WallrunCheck();

public:
	//How close to the wall the player must be to start the wallrun
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float WallrunCheckDistance= 120;
	
	

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

//UserInterface/UI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD");
	TSubclassOf<UUserWidget> MainHud;
	
	
};
