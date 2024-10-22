// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleShooter.generated.h"


class UCableComponent;
class UCameraComponent;
class AGrappleProjectile;

UENUM(BlueprintType)
enum class EGrappleState :uint8
{
	Standby UMETA(DisplayName= "Standby"),
	ShootingOut UMETA(DisplayName= "Shooting out"),
	Pulling UMETA(DisplayName= "Pulling"),
	ReelingIn UMETA(DisplayName= "Reeling In"),
	CoolDown UMETA(DisplayName="Cool down"),
	SoftCoolDown UMETA(DisplayName="Soft cool down")
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
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(UCameraComponent* PlayerCamera);

	UFUNCTION()
	void StartPulling();
	
	void LetGo();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GrappleShooterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGrappleState State = EGrappleState::Standby;
protected:
	AGrapplePlayerCharacter* MyCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AGrappleProjectile> BpProjectileClass;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Range= 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float PullStrenght= 500000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DetachDistanceThreshold= 150;
	
	//max time to reel in the hook in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxReelInTime= 2;

	//minimum distance for max reel in time in cm
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxReelInTimeDistance= 5000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CoolDownTime =1;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCableComponent* CableComponent;

	
private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetCurrentProjectile)
	AGrappleProjectile* CurrentProjectile;
public:
	UFUNCTION(BlueprintCallable)
	AGrappleProjectile* GetCurrentProjectile() const {return this->CurrentProjectile;}
private:

	float ReelingInSecondsSum=0;
	FVector InitalProjectileReelInPosition;
	float ReelInTime=0;
	void StartCoolDown();
	float CoolDownTimeCounter=0;

public:
	UFUNCTION(BlueprintCallable)
	void StartSoftCooldown();

	UFUNCTION(BlueprintCallable)
	void SoftCooldownOver();
	
};
