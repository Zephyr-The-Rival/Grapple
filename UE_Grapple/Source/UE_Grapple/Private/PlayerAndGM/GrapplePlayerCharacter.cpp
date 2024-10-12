
#include "UE_Grapple/Public/PlayerAndGM/GrapplePlayerCharacter.h"
#include "Debug.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"


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

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetController()->GetNetOwningPlayer());
	
	
	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	InputSystem->AddMappingContext(DefaultMappingContext, InputPriority);

	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Look);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Move);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGrapplePlayerCharacter::Jump);
	
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

void AGrapplePlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d Vector2d= Value.Get<FVector2d>();
	FVector DeltaMovement= FVector(Vector2d.X,Vector2d.Y,0);
	FVector Forward = GetCapsuleComponent()->GetForwardVector()*Vector2d.X;
	FVector Right = GetCapsuleComponent()->GetRightVector()*Vector2d.Y;
	
	GetMovementComponent()->AddInputVector(Forward+Right);
}



