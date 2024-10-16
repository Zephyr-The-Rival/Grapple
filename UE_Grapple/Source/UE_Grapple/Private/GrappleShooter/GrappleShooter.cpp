// Fill out your copyright notice in the Description page of Project Settings.

#include "GrappleShooter/GrappleShooter.h"
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "GrappleShooter/GrappleProjectile.h"


// Sets default values
AGrappleShooter::AGrappleShooter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->GrappleShooterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AGrappleShooter::BeginPlay()
{
	Super::BeginPlay();
	if(AGrapplePlayerCharacter* Character=Cast<AGrapplePlayerCharacter>(GetParentActor()))
	{
		this->MyCharacter=Character;
	}
	else
	{
		Debug::Print("GrappleShooter is not parented to a player Character");
		this->Destroy();
	}
	
}

// Called every frame
void AGrappleShooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGrappleShooter::Shoot(FVector HitCoordinate)
{
	this->State=EGrappleState::ShootingOut;

	FVector Location= this->GrappleShooterMesh->GetSocketLocation("ShootSocket");
	FRotator Rotation= (HitCoordinate-Location).Rotation();

	GetWorld()->SpawnActor<AGrappleProjectile>(Location,Rotation);
}

void AGrappleShooter::LetGo()
{
	if(State!=EGrappleState::Pulling)
		return;

	Debug::Print("StoppingGrapple...");
}

