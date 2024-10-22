

#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GrappleShooter/GrappleShooter.h"
#include "PlayerAndGM/GrapplePC.h"


// Sets default values
AGrapplePlayerCharacter::AGrapplePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(this->GetMesh(), TEXT("CameraSocket"));

	this->GrappleShooterChildActor= CreateDefaultSubobject<UChildActorComponent>("GrappleShooterChildActor");
	this->GrappleShooterChildActor->SetupAttachment(this->GetMesh(),TEXT("lowerarm_l"));
}

// Called when the game starts or when spawned
void AGrapplePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetDefaultValues();

	if(MainHud)
		CreateWidget<UUserWidget>(Cast<AGrapplePC>(GetController()),MainHud)->AddToViewport(0);
	if(DebugStats)
		CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()),DebugStats)->AddToViewport(1);
	
	
}

// Called every frame
void AGrapplePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(this->bIsBoosting)
		TickApplyBoost();

	if(this->bRefuelBoostAllowed)
		TickRefuelBoost();

	Debug::Print("BoostingFuel: "+FString::SanitizeFloat(this->BoostingFuel),DeltaTime);
		
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

	Input->BindAction(ShootGrapplingHookAction, ETriggerEvent::Started, this, &AGrapplePlayerCharacter::ShootGrapplePressed);
	Input->BindAction(ShootGrapplingHookAction, ETriggerEvent::Completed, this, &AGrapplePlayerCharacter::ShootGrappleEnd);
	
}

void AGrapplePlayerCharacter::Look(const FInputActionValue& Value)
{
	float DeltaYaw = Value.Get<FVector2d>().X * this->TurningSpeed;
	AddControllerYawInput(DeltaYaw);

	float DeltaPitch = Value.Get<FVector2d>().Y * this->TurningSpeed;
	FRotator Rotator = FRotator(DeltaPitch,0,0);
	if(abs(GetMesh()->GetRelativeRotation().Pitch+DeltaPitch)<this->PitchLimit)
		GetMesh()->AddLocalRotation(Rotator);
	
}

void AGrapplePlayerCharacter::JumpButtonDown()
{
	if(GetMovementComponent()->IsFalling())
	{
		StartBoosting();
	}
	else
	{
		Jump();//part of character;
		OnStartJump.Broadcast();
		this->bJumping=true;
	}
	
}

void AGrapplePlayerCharacter::EndJump()
{
	if(bIsBoosting)
	{
		StopBoosting();
	}
	else
	{
		StopJumping();//part of character;
		OnEndJump.Broadcast();
		this->bJumping=false;
	}
}

void AGrapplePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d Vector2d= Value.Get<FVector2d>();

	if(Vector2d.X<0.5)
	{
		StopSprinting();
	}
	
	FVector DeltaMovement= FVector(Vector2d.X,Vector2d.Y,0);
	FVector Forward = GetCapsuleComponent()->GetForwardVector()*Vector2d.X;
	FVector Right = GetCapsuleComponent()->GetRightVector()*Vector2d.Y;
	
	GetMovementComponent()->AddInputVector(Forward+Right);
}

void AGrapplePlayerCharacter::SprintButtonDown()
{
	AGrapplePC* PlayerController= Cast<AGrapplePC>(GetController());
	if(PlayerController->bIsUsingGamepad)
	{
		if(this->bIsSprinting)
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
	AGrapplePC* PlayerController= Cast<AGrapplePC>(GetController());
	if(!PlayerController->bIsUsingGamepad)
		StopSprinting();
	
}

void AGrapplePlayerCharacter::StartSprinting()
{
	this->bIsSprinting=true;
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MaxWalkSpeed=this->SprintingSpeed;
}

void AGrapplePlayerCharacter::StopSprinting()
{
	this->bIsSprinting=false;
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MaxWalkSpeed=this->WalkingSpeed;
}

void AGrapplePlayerCharacter::StartBoosting()
{
	this->bIsBoosting=true;
	this->bRefuelBoostAllowed=false;
	GetWorld()->GetTimerManager().PauseTimer(BoostRefuelTimer);
}

void AGrapplePlayerCharacter::StopBoosting()
{
	this->bIsBoosting=false;
	GetWorld()->GetTimerManager().SetTimer(BoostRefuelTimer,this,&AGrapplePlayerCharacter::AllowBoostRefill,BoostingRefuelDelay,false);
}

void AGrapplePlayerCharacter::TickApplyBoost()
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	if(BoostingFuel>0)
	{
		BoostingFuel-=GetWorld()->DeltaTimeSeconds;
		CharacterMovementComponent->AddForce(FVector(0,0,this->BoostingStrenght));//deltatime is adjusted for already
	}
	else
	{
		StopBoosting();
	}
}

void AGrapplePlayerCharacter::TickRefuelBoost()
{
	if(BoostingFuel==MaxBoostingFuel)
		return;
	
	if(BoostingFuel<MaxBoostingFuel)
		this->BoostingFuel+=GetWorld()->DeltaTimeSeconds*this->BoostingRefuelSpeed;
	else
		BoostingFuel=MaxBoostingFuel;
}

void AGrapplePlayerCharacter::AllowBoostRefill()
{
	this->bRefuelBoostAllowed=true;
}

void AGrapplePlayerCharacter::ShootGrapplePressed()
{
	if(!IsValid(MyGrappleShooter))
		return;

	if(!(MyGrappleShooter->State==EGrappleState::Standby || MyGrappleShooter->State==EGrappleState::SoftCoolDown))
		return;

	MyGrappleShooter->State=EGrappleState::ShootingOut;//will trigger the animatoin that will shoot the projectile
	
}

void AGrapplePlayerCharacter::ShootGrappleEnd()
{
	if(!IsValid(MyGrappleShooter))
		return;
	
	MyGrappleShooter->LetGo();
}

void AGrapplePlayerCharacter::SetDefaultValues()
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MaxWalkSpeed=this->WalkingSpeed;

	this->MyGrappleShooter= Cast<AGrappleShooter>(GrappleShooterChildActor->GetChildActor());
	this->BoostingFuel=this->MaxBoostingFuel;
}



