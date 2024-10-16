// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleShooter.generated.h"


UENUM(BlueprintType)
enum class EGrappleState :uint8
{
	Standby UMETA(DisplayName= "Standby"),
	ShootingOut UMETA(DisplayName= "Shooting out"),
	Pulling UMETA(DisplayName= "Pulling"),
	ReelingIn UMETA(DisplayName= "Reeling In")
};


class AGrapplePlayerCharacter;

UCLASS()
class UE_GRAPPLE_API AGrappleShooter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrappleShooter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(FVector HitCoordinate);
	void LetGo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GrappleShooterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGrappleState State = EGrappleState::Standby;
protected:
	AGrapplePlayerCharacter* MyCharacter;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Range= 3000;
	
};