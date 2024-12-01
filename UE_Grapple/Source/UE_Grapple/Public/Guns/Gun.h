// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class EGunState: uint8
{
	Idle UMETA(DisplayName="Idle"),
	Shooting UMETA(DisplayName="Shooting"),
	Reloading UMETA(DisplayName="Reloading")
};
UCLASS()
class UE_GRAPPLE_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAutomatic=false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FireInterval=0.5;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int MaxAmmo=6;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int Ammo=6;

public:
	void PullTrigger();
	void ReleaseTrigger();
	void Reload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunState State=EGunState::Idle;
private:
	bool bFireReady=true;
	void Shoot();
	void FinishShootCycle();
	bool bFirstShot=true;
	
};
