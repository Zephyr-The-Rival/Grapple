// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrappleMovementComponent.generated.h"

class AGrapplePlayerCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_WallRun UMETA(DisplayName="Wall Run"),
	CMOVE_MAX UMETA(Hidden)
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UE_GRAPPLE_API UGrappleMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrappleMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

protected:
	UPROPERTY(Transient)
	AGrapplePlayerCharacter* MyGrapplePlayerCharacter;

	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintPure)
	bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode)const;

public:
	
	void StartWallrun(bool bRightSide);
	void EndWallrun();
	
private:
	bool bWallrunRight;
	
	void PhysWallrun(float deltaTime, int32 Iterations);

	FHitResult WallrunLineTrace(bool bRight);
public:
	//CustomParameters

	//wallrun time in seconds
	UPROPERTY(EditDefaultsOnly) float WallrunGravity=300;
	
	UPROPERTY(EditDefaultsOnly) float WallrunMinSpeed=600;
};


