#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GrappleShooter/GrappleShooter.h"
#include "PlayerAndGM/GrappleMovementComponent.h"
#include "PlayerAndGM/GrapplePC.h"


// Sets default values
AGrapplePlayerCharacter::AGrapplePlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UGrappleMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	this->CharacterMovementComponent= Cast<UGrappleMovementComponent>(GetMovementComponent());
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(this->GetMesh(), TEXT("CameraSocket"));

	this->GrappleShooterChildActor = CreateDefaultSubobject<UChildActorComponent>("GrappleShooterChildActor");
	this->GrappleShooterChildActor->SetupAttachment(this->GetMesh(),TEXT("lowerarm_l"));

	
}

// Called when the game starts or when spawned
void AGrapplePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetDefaultValues();

	if (MainHud)
		CreateWidget<UUserWidget>(Cast<AGrapplePC>(GetController()), MainHud)->AddToViewport(0);
	if (DebugStats)
		CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()), DebugStats)->AddToViewport(1);
}

// Called every frame
void AGrapplePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bIsBoosting)
		Tick_ApplyBoost();

	if (this->bRefuelBoostAllowed)
		Tick_RefuelBoost();

	if(CharacterMovementComponent->MovementMode!=ECustomMovementMode::CMOVE_WallRun)
		Tick_WallrunCheck();
}

// Called to bind functionality to input
void AGrapplePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController()->GetNetOwningPlayer());


	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->AddMappingContext(DefaultMappingContext_MaK, InputPriority);
	InputSystem->AddMappingContext(DefaultMappingContext_GamePad, InputPriority);


	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Look);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Move);

	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGrapplePlayerCharacter::JumpButtonDown);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGrapplePlayerCharacter::EndJump);

	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGrapplePlayerCharacter::SprintButtonDown);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGrapplePlayerCharacter::SprintButtonUp);

	Input->BindAction(ShootGrapplingHookAction, ETriggerEvent::Started, this,&AGrapplePlayerCharacter::ShootGrapplePressed);
	Input->BindAction(ShootGrapplingHookAction, ETriggerEvent::Completed, this,&AGrapplePlayerCharacter::ShootGrappleEnd);
}


void AGrapplePlayerCharacter::Look(const FInputActionValue& Value)
{
	float DeltaYaw = Value.Get<FVector2d>().X * this->TurningSpeed;
	AddControllerYawInput(DeltaYaw);

	float DeltaPitch = Value.Get<FVector2d>().Y * this->TurningSpeed;
	FRotator Rotator = FRotator(DeltaPitch, 0, 0);
	if (abs(GetMesh()->GetRelativeRotation().Pitch + DeltaPitch) < this->PitchLimit)
		GetMesh()->AddLocalRotation(Rotator);
}

void AGrapplePlayerCharacter::JumpButtonDown()
{
	if (GetMovementComponent()->IsFalling())
	{
		StartBoosting();
	}
	else
	{
		Jump(); //part of character;
		OnStartJump.Broadcast();
		this->bJumping = true;
	}
}

void AGrapplePlayerCharacter::EndJump()
{
	if (bIsBoosting)
	{
		StopBoosting();
	}
	else
	{
		StopJumping(); //part of character;
		OnEndJump.Broadcast();
		this->bJumping = false;
	}
}

void AGrapplePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d Vector2d = Value.Get<FVector2d>();
	
	FVector DeltaMovement = FVector(Vector2d.X, Vector2d.Y, 0);
	FVector Forward = GetCapsuleComponent()->GetForwardVector() * Vector2d.X;
	FVector Right = GetCapsuleComponent()->GetRightVector() * Vector2d.Y;

	GetMovementComponent()->AddInputVector(Forward + Right);

	 this->CurrentMovementDirection.bForward=(Vector2d.X>0.5);
	 this->CurrentMovementDirection.bBackward=(Vector2d.X<-0.5);
	 this->CurrentMovementDirection.bRight=(Vector2d.Y>0.5);
	 this->CurrentMovementDirection.bLeft=(Vector2d.Y<-0.5);

	if (Vector2d.X<0.5)
	{
		StopSprinting();
	}

}

void AGrapplePlayerCharacter::SprintButtonDown()
{
	AGrapplePC* PlayerController = Cast<AGrapplePC>(GetController());
	if (PlayerController->bIsUsingGamepad)
	{
		if (this->bIsSprinting)
			StopSprinting();
		else
			StartSprinting();
	}
	else
	{
		StartSprinting();
	}
}

void AGrapplePlayerCharacter::SprintButtonUp()
{
	AGrapplePC* PlayerController = Cast<AGrapplePC>(GetController());
	if (!PlayerController->bIsUsingGamepad)
		StopSprinting();
}

void AGrapplePlayerCharacter::StartSprinting()
{
	this->bIsSprinting = true;
	CharacterMovementComponent->MaxWalkSpeed = this->SprintingSpeed;
}

void AGrapplePlayerCharacter::StopSprinting()
{
	this->bIsSprinting = false;
	CharacterMovementComponent->MaxWalkSpeed = this->WalkingSpeed;
}

void AGrapplePlayerCharacter::StartBoosting()
{
	this->bIsBoosting = true;
	this->bRefuelBoostAllowed = false;
	GetWorld()->GetTimerManager().PauseTimer(BoostRefuelTimer);
}

void AGrapplePlayerCharacter::StopBoosting()
{
	this->bIsBoosting = false;
	GetWorld()->GetTimerManager().SetTimer(BoostRefuelTimer, this, &AGrapplePlayerCharacter::AllowBoostRefill,BoostingRefuelDelay, false);
}

void AGrapplePlayerCharacter::Tick_ApplyBoost()
{
	if (BoostingFuel > 0)
	{
		BoostingFuel -= GetWorld()->DeltaTimeSeconds;
		CharacterMovementComponent->AddForce(FVector(0, 0, this->BoostingStrenght)); //deltatime is adjusted for already
	}
	else
	{
		StopBoosting();
	}
}

void AGrapplePlayerCharacter::Tick_RefuelBoost()
{
	if (BoostingFuel == MaxBoostingFuel)
		return;

	if (BoostingFuel < MaxBoostingFuel)
		this->BoostingFuel += GetWorld()->DeltaTimeSeconds * this->BoostingRefuelSpeed;
	else
		BoostingFuel = MaxBoostingFuel;
}

void AGrapplePlayerCharacter::AllowBoostRefill()
{
	this->bRefuelBoostAllowed = true;
}

void AGrapplePlayerCharacter::Tick_WallrunCheck()
{
	
	if (!GetMovementComponent()->IsFalling())
		return;

	FHitResult HitResultRight;

	FVector Start= this->GetActorLocation();
	FVector End =Start+ this->GetActorRightVector() * this->WallrunCheckDistance;

	
	GetWorld()->LineTraceSingleByChannel(HitResultRight,Start,End,ECC_GameTraceChannel1);


	float dot=GetActorForwardVector().Dot(HitResultRight.Normal);
	if (HitResultRight.bBlockingHit && dot < -0.1f && dot > -0.8f && CurrentMovementDirection.bForward)
	{
		Debug::Print("Hitting stuff on the right: "+FString::SanitizeFloat(dot), GetWorld()->GetDeltaSeconds());
		CharacterMovementComponent->StartWallrun(true);
	}

	FHitResult HitResultLeft;
	End =Start+ this->GetActorRightVector() * this->WallrunCheckDistance* -1;

	
	GetWorld()->LineTraceSingleByChannel(HitResultLeft,Start,End,ECC_GameTraceChannel1);

	dot=GetActorForwardVector().Dot(HitResultLeft.Normal);
	if(HitResultLeft.bBlockingHit && dot< -0.1f && dot >-0.8f && CurrentMovementDirection.bForward)
	{
		Debug::Print("Hitting stuff on the Left: "+FString::SanitizeFloat(dot), GetWorld()->DeltaTimeSeconds);
		CharacterMovementComponent->StartWallrun(false);
	}
}

void AGrapplePlayerCharacter::ShootGrapplePressed()
{
	if (!IsValid(MyGrappleShooter))
		return;

	if (!(MyGrappleShooter->State == EGrappleState::Standby || MyGrappleShooter->State == EGrappleState::SoftCoolDown))
		return;

	MyGrappleShooter->State = EGrappleState::ShootingOut; //will trigger the animatoin that will shoot the projectile
}

void AGrapplePlayerCharacter::ShootGrappleEnd()
{
	if (!IsValid(MyGrappleShooter))
		return;

	MyGrappleShooter->LetGo();
}

void AGrapplePlayerCharacter::SetDefaultValues()
{
	CharacterMovementComponent->MaxWalkSpeed = this->WalkingSpeed;

	this->MyGrappleShooter = Cast<AGrappleShooter>(GrappleShooterChildActor->GetChildActor());
	this->BoostingFuel = this->MaxBoostingFuel;
}
