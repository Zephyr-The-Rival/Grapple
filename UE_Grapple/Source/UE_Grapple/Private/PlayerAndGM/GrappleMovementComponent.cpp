// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAndGM/GrappleMovementComponent.h"

#include "Debug.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"
#include "PlayerAndGM/GrapplePlayerCharacter.h"


// Sets default values for this component's properties
UGrappleMovementComponent::UGrappleMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrappleMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrappleMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	switch (CustomMovementMode)
	{
	case ECustomMovementMode::CMOVE_WallRun:
		PhysWallrun(deltaTime, Iterations);
		break;
		
	default:
		break;
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void UGrappleMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	this->MyGrapplePlayerCharacter= Cast<AGrapplePlayerCharacter>(GetOwner());
}

bool UGrappleMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MOVE_Custom && CustomMovementMode == InCustomMovementMode;
}

void UGrappleMovementComponent::StartWallrun(bool bRightSide)
{
	Debug::Print("Staring wallrun");
	GravityScale=0;
	this->bWallrunRight=bRightSide;
	
	SetMovementMode(MOVE_Custom, CMOVE_WallRun);
}

void UGrappleMovementComponent::PhysWallrun(float deltaTime, int32 Iterations)
{
	if(deltaTime< MIN_TICK_TIME)
		return;
	
	RestorePreAdditiveRootMotionVelocity();

	FHitResult HitResult =WallrunLineTrace(bWallrunRight); 
	if(!HitResult.bBlockingHit)
		EndWallrun();

	float vectorRotation;
	if(bWallrunRight)
		vectorRotation=90;
	else
		vectorRotation=-90;
	
	FVector RunDirection = HitResult.Normal.RotateAngleAxis(vectorRotation,FVector(0,0,1));
	
	Velocity = RunDirection*WallrunMinSpeed*deltaTime;
	
	//if(Velocity.Length()<WallrunMinSpeed)
		
	
	FQuat AbsoluteRotation =FQuat::MakeFromRotator(MyGrapplePlayerCharacter->GetActorRotation());
	
	Iterations++;
	bJustTeleported=false;
	FHitResult SweepHit= FHitResult();
	SafeMoveUpdatedComponent(Velocity, AbsoluteRotation, true,SweepHit);
	
	// if(SweepHit.bBlockingHit)
	// 	HandleImpact(SweepHit, deltaTime, DeltaPosition);
}

FHitResult UGrappleMovementComponent::WallrunLineTrace(bool bRight)
{
	FVector directionVector=MyGrapplePlayerCharacter->GetActorRightVector();

	if(!bRight)
		directionVector*=-1;
	
	
	
	FHitResult HitResult;

	FVector Start= this->GetActorLocation();
	FVector End =Start+ directionVector * MyGrapplePlayerCharacter->WallrunCheckDistance;

	
	GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_GameTraceChannel1);
	return HitResult;
}

void UGrappleMovementComponent::EndWallrun()
{
	Debug::Print("Ending wallrun");
	GravityScale=GetDefault<UGrappleMovementComponent>()->GravityScale;
	SetMovementMode(MOVE_Walking);
}

