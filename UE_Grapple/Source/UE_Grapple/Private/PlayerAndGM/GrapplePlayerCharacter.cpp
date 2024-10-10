﻿
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"


// Sets default values
AGrapplePlayerCharacter::AGrapplePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(this->GetMesh(), TEXT("CameraSocket"));
}

// Called when the game starts or when spawned
void AGrapplePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrapplePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGrapplePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
