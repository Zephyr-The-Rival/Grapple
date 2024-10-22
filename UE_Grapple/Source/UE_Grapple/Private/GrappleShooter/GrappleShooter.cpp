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
			Force= Force*this->PullStrenght;
			MyCharacter->GetCharacterMovement()->AddForce(Force);//deltatime is adjusted for already
			
		}
		
	}
	if(this->State==EGrappleState::ReelingIn)
	{	
		float LerpAlpha=0;
		this->ReelingInSecondsSum+=DeltaTime;

		LerpAlpha= ReelingInSecondsSum/ReelInTime;
		if(LerpAlpha>=1)
			LerpAlpha=1;
		
		CurrentProjectile->SetActorLocation(FMath::Lerp(InitalProjectileReelInPosition, this->GetActorLocation(),LerpAlpha));
		

		float currentDistance = FVector::Distance(this->GetActorLocation(), CurrentProjectile->GetActorLocation());
		if (currentDistance < 10)//|| currentDistance > lastDistance)
		{
			StartCoolDown();
		}
		
	}
	if(State==EGrappleState::CoolDown)
	{
		this->CoolDownTimeCounter+=DeltaTime;
		if(this->CoolDownTimeCounter>=this->CoolDownTime)
		{
			State=EGrappleState::Standby;
		}
	}
	
}

void AGrappleShooter::SpawnProjectile(UCameraComponent* PlayerCamera)
{
	if(this->State!=EGrappleState::ShootingOut)//to make sure because animation blending can cause this to be triggered during a different state
		return;
	
	FHitResult HitResult;

	FVector Start= PlayerCamera->GetComponentLocation();
	FVector End= Start+ PlayerCamera->GetForwardVector()*this->Range;

	GetWorld()->LineTraceSingleByChannel(HitResult,Start, End, ECC_Visibility);

	
	FVector SpawnLocation = this->GrappleShooterMesh->GetSocketLocation("ShootSocket");
	FRotator SpawnRotation;
	
	if(HitResult.bBlockingHit)
	{
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
	{
		this->State=EGrappleState::SoftCoolDown;
		return;
	}
		

	FDetachmentTransformRules DetachmentRules=FDetachmentTransformRules(EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,EDetachmentRule::KeepWorld,false);
	CurrentProjectile->DetachFromActor(DetachmentRules);
	CurrentProjectile->StopMove();

	this->ReelingInSecondsSum=0;
	InitalProjectileReelInPosition= CurrentProjectile->GetActorLocation();

	float ReelInTimeAlpha= FVector::Distance(this->GetActorLocation(),CurrentProjectile->GetActorLocation())/MaxReelInTimeDistance;
	this->ReelInTime=FMath::Lerp(0,MaxReelInTime,ReelInTimeAlpha); //is used on tick while reeling in
	
	State=EGrappleState::ReelingIn;
}

void AGrappleShooter::StartCoolDown()
{
	this->CableComponent->SetHiddenInGame(true);
	CurrentProjectile->Destroy();
	CurrentProjectile=nullptr;//destroy doesnt seem to be enough
	this->CoolDownTimeCounter=0;
	this->State = EGrappleState::CoolDown;
}

void AGrappleShooter::StartSoftCooldown()
{
	this->State=EGrappleState::SoftCoolDown;
}

void AGrappleShooter::SoftCooldownOver()
{
	this->State=EGrappleState::Standby;
}

