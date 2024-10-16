
#include "GrappleShooter/GrappleShooter.h"
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"



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

	//if(DebugStats)
	//	CreateWidget<UUserWidget>(Cast<APlayerController>(GetController()),DebugStats)->AddToViewport(); 
	
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

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController()->GetNetOwningPlayer());
	
	
	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->AddMappingContext(DefaultMappingContext, InputPriority);

	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Look);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Move);
	
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGrapplePlayerCharacter::StartJump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGrapplePlayerCharacter::EndJump);
	
	Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGrapplePlayerCharacter::StartSprinting);
	Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGrapplePlayerCharacter::StopSprinting);

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

void AGrapplePlayerCharacter::StartJump()
{
	Jump();//part of character;
	bJumpButtonDown=true;
	OnStartJump.Broadcast();
}

void AGrapplePlayerCharacter::EndJump()
{
	StopJumping();//part of character;
	bJumpButtonDown=false;
	OnEndJump.Broadcast();
}

void AGrapplePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d Vector2d= Value.Get<FVector2d>();
	FVector DeltaMovement= FVector(Vector2d.X,Vector2d.Y,0);
	FVector Forward = GetCapsuleComponent()->GetForwardVector()*Vector2d.X;
	FVector Right = GetCapsuleComponent()->GetRightVector()*Vector2d.Y;
	
	GetMovementComponent()->AddInputVector(Forward+Right);
}

void AGrapplePlayerCharacter::StartSprinting()
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MaxWalkSpeed=this->SprintingSpeed;
}

void AGrapplePlayerCharacter::StopSprinting()
{
	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MaxWalkSpeed=this->WalkingSpeed;
}

void AGrapplePlayerCharacter::ShootGrapplePressed()
{
	if(!IsValid(MyGrappleShooter))
		return;

	if(MyGrappleShooter->State!=EGrappleState::Standby)
		return;

	FHitResult HitResult;

	FVector Start= Camera->GetComponentLocation();
	FVector End= Start+ Camera->GetForwardVector()*MyGrappleShooter->Range;

	GetWorld()->LineTraceSingleByChannel(HitResult,Start, End, ECC_Visibility);

	FVector HitLocation;
	if(HitResult.bBlockingHit)
	{
		Debug::Print("Grapple Trace  hit:"+ HitResult.GetActor()->GetName()+" at: "+ HitResult.Location.ToString());
		HitLocation= HitResult.Location;
	}
	else
	{
		HitLocation=End;
	}
	

	MyGrappleShooter->Shoot(HitLocation);
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
}



