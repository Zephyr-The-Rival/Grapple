// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingTarget.generated.h"

UENUM(BlueprintType)
enum class ETargetState: uint8
{
	Idle UMETA(DisplayName="Idle"),
	LookingForPosition UMETA(DisplayName="Looking for Position"),
	MovingToNewPosition UMETA(DisplayName="Moving to new Position")

};
UCLASS()
class UE_GRAPPLE_API AFlyingTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlyingTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;


	
private:
	ETargetState State;
	FVector NewLoaction;
	FVector NormalizedMovementDirection;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MovementSpeed=300;

private:

	void LookForNewLocation();
	void ArrivedAtNewLocation();
	FHitResult PerformCapsuleTrace(FVector EndPoint);
	TArray<FVector> GenerateSearchPoints();
};
