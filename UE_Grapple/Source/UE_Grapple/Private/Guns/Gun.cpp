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
	
	if(Ammo==0)
	{
		Reload();
		return;
	}

	if(State==EGunState::Idle||State==EGunState::SoftCoolDown)
		Shoot();
}

void AGun::Shoot()
{
	bFirstShot=false;
	State=EGunState::Shooting;
	Ammo--;
}




void AGun::ReleaseTrigger()
{
	bFirstShot=true;
}

void AGun::Reload()
{
	State=EGunState::Reloading;
}

void AGun::SetGunState(EGunState NewState)
{
	State=NewState;
}


