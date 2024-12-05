// Fill out your copyright notice in the Description page of Project Settings.


#include "Guns/Gun.h"

#include "Debug.h"


// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GunMesh=CreateDefaultSubobject<USkeletalMeshComponent>("GunMesh");
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AGun::PullTrigger()
{
	if(!bFirstShot && !bAutomatic)
		return;
	
	this->Shoot();
}

void AGun::Shoot()
{
	bFirstShot=false;
	SetGunState(EGunState::Shooting);
	Ammo--;
}




void AGun::ReleaseTrigger()
{
	bFirstShot=true;
}

void AGun::RefillAmmo()
{
	this->Ammo=this->MaxAmmo;
}


void AGun::SetGunState(EGunState NewState)
{
	State=NewState;
	OnGunStateChanged.Broadcast(NewState);
}


