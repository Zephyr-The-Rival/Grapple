// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleShooter/GrappleProjectile.h"



// Sets default values
AGrappleProjectile::AGrappleProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Mesh= CreateDefaultSubobject<UMeshComponent>(TEXT("HookMesh"));
}

// Called when the game starts or when spawned
void AGrappleProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrappleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(this->Move)
	{
		FHitResult ProjectileHitResult;
		this->AddActorWorldOffset(GetActorForwardVector()*this->ProjectileSpeed, true,&ProjectileHitResult);

		if(ProjectileHitResult.bBlockingHit)
		{
			OnHit.Broadcast();
			this->Move=false;
			this->SetActorLocation(ProjectileHitResult.Location);
		}
	}
	
}

