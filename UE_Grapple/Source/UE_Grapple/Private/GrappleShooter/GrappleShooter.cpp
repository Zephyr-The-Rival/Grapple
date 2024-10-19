// Fill out your copyright notice in the Description page of Project Settings.

#include "GrappleShooter/GrappleShooter.h"
#include "GrappleShooter/GrappleShooter.h"
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "GrappleShooter/GrappleProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CableComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGrappleShooter::AGrappleShooter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->GrappleShooterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	this->CableComponent= CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	this->CableComponent->SetupAttachment(GrappleShooterMesh,"ShootSocket");
	this->CableComponent->SetHiddenInGame(true);
	
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

	if(!IsValid(MyCharacter))
		return;

	if (this->State == EGrappleState::ShootingOut && IsValid(CurrentProjectile))
	{
		if(FVector::Distance(MyCharacter->GetActorLocation(), CurrentProjectile->GetActorLocation())>this->Range)
		{
			this->LetGo();
		}
	}
	if(this->State==EGrappleState::Pulling)
	{
		float currentDistance=FVector::Distance(MyCharacter->GetActorLocation(), CurrentProjectile->GetActorLocation());
		if(currentDistance<DetachDistanceThreshold)
		{
			LetGo();
		}
		else
		{
			FVector Force = CurrentProjectile->GetActorLocation()-MyCharacter->GetActorLocation();
			Force.Normalize();
			Force= Force*this->PullStrenght*DeltaTime;
			MyCharacter->GetCharacterMovement()->AddForce(Force);
			
		}
		
	}
	if(this->State==EGrappleState::ReelingIn)
	{
		float currentDistance = FVector::Distance(this->GetActorLocation(), CurrentProjectile->GetActorLocation());
		if (currentDistance < 150)//|| currentDistance > lastDistance)
		{
			this->CableComponent->SetHiddenInGame(true);
			CurrentProjectile->Destroy();
			this->State = EGrappleState::Standby;
		}
		else
		{
			FVector direction = this->GetActorLocation() - CurrentProjectile->GetActorLocation();
			direction.Normalize();
			CurrentProjectile->AddActorWorldOffset(direction * this->ReelInSpeed * DeltaTime);
		}
		
	}
	
}

void AGrappleShooter::SpawnProjectile(UCameraComponent* PlayerCamera)
{

	FHitResult HitResult;

	FVector Start= PlayerCamera->GetComponentLocation();
	FVector End= Start+ PlayerCamera->GetForwardVector()*this->Range;

	GetWorld()->LineTraceSingleByChannel(HitResult,Start, End, ECC_Visibility);

	
	FVector SpawnLocation = this->GrappleShooterMesh->GetSocketLocation("ShootSocket");
	FRotator SpawnRotation;
	
	if(HitResult.bBlockingHit)
	{
		Debug::Print("Grapple Trace  hit:"+ UKismetSystemLibrary::GetDisplayName(HitResult.GetActor())+" at: "+ HitResult.Location.ToString());
		SpawnRotation= (HitResult.Location-SpawnLocation).Rotation();
	}
	else
		SpawnRotation=MyCharacter->Camera->GetComponentRotation();
	
	
	this->CurrentProjectile=GetWorld()->SpawnActor<AGrappleProjectile>(BpProjectileClass,SpawnLocation,SpawnRotation);
	this->CableComponent->SetHiddenInGame(false);
	this->CableComponent->SetAttachEndTo(CurrentProjectile, "Mesh");

	CurrentProjectile->OnHit.AddUFunction(this, "StartPulling");
	
}

void AGrappleShooter::StartPulling()
{
	this->State=EGrappleState::Pulling;
}

void AGrappleShooter::LetGo()
{
	if(State!=EGrappleState::Pulling && State!=EGrappleState::ShootingOut)
		return;

	if(!CurrentProjectile)
		return;

	FDetachmentTransformRules DetachmentRules=FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,false);
	CurrentProjectile->DetachFromActor(DetachmentRules);
	CurrentProjectile->StopMove();
	
	State=EGrappleState::ReelingIn;
}

