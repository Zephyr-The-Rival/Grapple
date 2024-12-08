// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingTarget.h"

#include "PlayerAndGM/GrapplePlayerCharacter.h"


// Sets default values
AFlyingTarget::AFlyingTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
}

// Called when the game starts or when spawned
void AFlyingTarget::BeginPlay()
{
	Super::BeginPlay();
	State=ETargetState::Idle;
	LookForNewLocation();
	
}

// Called every frame
void AFlyingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(State==ETargetState::MovingToNewPosition)
	{
		if(FVector::Distance(GetActorLocation(), NewLoaction)<10)
		{
			ArrivedAtNewLocation();
		}
			

		AddActorWorldOffset(NormalizedMovementDirection*DeltaTime*MovementSpeed);
	}
}

void AFlyingTarget::LookForNewLocation()
{

	TArray<FVector>SearchPoints= GenerateSearchPoints();
	TArray<FVector>ViablePoints;

	for (FVector SearchPoint : SearchPoints)
	{
		FHitResult hit=PerformCapsuleTrace(SearchPoint);
		if(!hit.bBlockingHit)
			ViablePoints.Add(SearchPoint);
	}

	NewLoaction=ViablePoints[FMath::FRandRange(0.0f, ViablePoints.Num()-1)];
	NormalizedMovementDirection=NewLoaction-GetActorLocation();
	NormalizedMovementDirection.Normalize();
	
	State = ETargetState::MovingToNewPosition;
}

void AFlyingTarget::ArrivedAtNewLocation()
{
	SetActorLocation(NewLoaction);
	State=ETargetState::Idle;

	FTimerHandle IdleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(IdleTimerHandle,this, &AFlyingTarget::LookForNewLocation, FMath::FRandRange(1.0,2.0),false);
}

FHitResult AFlyingTarget::PerformCapsuleTrace(FVector EndPoint)
{
	FHitResult HitResult;

	
	float CapsuleRadius = 50.0f; // Default radius
	float CapsuleHalfHeight = 50.0f; // Default half height
	ECollisionChannel TraceChannel = ECC_Visibility; // Default trace channel

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true; // Enable this for more detailed collision checks if necessary
	QueryParams.AddIgnoredActor(this);

	// Perform the trace
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		EndPoint,
		FQuat::Identity, // Orientation of the capsule; adjust if needed
		TraceChannel,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		QueryParams
	);

	// Optionally draw debug visuals
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), GetActorLocation(), CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, DebugColor, false, 2.0f);
	DrawDebugCapsule(GetWorld(), EndPoint, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, DebugColor, false, 2.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), EndPoint, DebugColor, false, 2.0f, 0, 2.0f);

	return HitResult;
}

TArray<FVector> AFlyingTarget::GenerateSearchPoints()
{
	TArray<FVector> Points;

	int levels=5;
	float SpaceBetweenLevels=200;
	float ZValue= GetActorLocation().Z-(levels/2)*SpaceBetweenLevels; //starting height for generating circles

	if(levels%2==0)
		ZValue+=SpaceBetweenLevels/2.0f;
	
	int PointsInCircle=6;
	float AngleStepSize=360/PointsInCircle;
	int Radius=300;

	int NumOfCirclesPerLevel=3;
	
	for (int l = 0; l < levels; l++)
	{
		for(int i=1; i<=NumOfCirclesPerLevel; i++)
		{
			for (int c = 0; c < PointsInCircle; c++)
			{
				FVector RotationCenter= GetActorLocation();
				RotationCenter.Z=ZValue;
				FVector point= RotationCenter+ FVector(Radius*i,0,0).RotateAngleAxis(AngleStepSize*c, FVector::UpVector);
				//DrawDebugCapsule(GetWorld(), point, 50, 50, FQuat::Identity, FColor::Cyan, false, 10.0f);
				Points.Add(point);
			}
		}
		
		ZValue += SpaceBetweenLevels;
	}
	
	return Points;
}

