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
	if(!bFireReady || !bFirstShot && !bAutomatic)
		return;
	
	if(Ammo==0)
	{
		Reload();
		return;
	}

	if(bFireReady)
		Shoot();
}

void AGun::Shoot()
{
	this->bFireReady=false;
	bFirstShot=false;
	State=EGunState::Shooting;
	Ammo--;
	FTimerHandle ShootCoolDown;
	GetWorld()->GetTimerManager().SetTimer(ShootCoolDown,this, &AGun::FinishShootCycle, this->FireInterval,false);
}

void AGun::FinishShootCycle()
{
	this->bFireReady=true;
	this->State=EGunState::Idle;
}


void AGun::ReleaseTrigger()
{
	bFirstShot=true;
}

void AGun::Reload()
{
	State=EGunState::Reloading;
}


