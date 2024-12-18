// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetBounds.generated.h"

class UBoxComponent;

UCLASS()
class UE_GRAPPLE_API ATargetBounds : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATargetBounds();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Volume;

public:
	bool IsLocationInBounds(FVector Location);
};
