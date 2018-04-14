// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CruxHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "CruxPlayerController.h"
#include "ProjectCrux.h"
#include "DrawDebugHelpers.h"


// Sets default values
ACruxCharacter::ACruxCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(SpringArmComp);

	HealthComp = CreateDefaultSubobject<UCruxHealthComponent>(TEXT("HealthComp"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	IsDead = false;
	IsAttacking = false;
	IsRotating = false;
	ActorName = "Player";
}

// Called when the game starts or when spawned
void ACruxCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ACruxCharacter::OnHealthChanged);
}

// Called every frame
void ACruxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRotating)
	{
		ACruxPlayerController* controller = Cast<ACruxPlayerController>(GetController());
		if (controller)
		{
			controller->SetMouseLocation(MouseX, MouseY);
		}
	}
}

// Called to bind functionality to input
void ACruxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACruxCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACruxCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACruxCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ACruxCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACruxCharacter::Jump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACruxCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACruxCharacter::EndCrouch);
	PlayerInputComponent->BindAction("AutoAttack", IE_Pressed, this, &ACruxCharacter::AutoAttack);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &ACruxCharacter::RightMousePressed);
	PlayerInputComponent->BindAction("RightMouse", IE_Released, this, &ACruxCharacter::RightMouseReleased);
	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ACruxCharacter::LeftMousePressed);
	PlayerInputComponent->BindAction("LeftMouse", IE_Released, this, &ACruxCharacter::LeftMouseReleased);
}

void ACruxCharacter::MoveForward(float val)
{
	AddMovementInput(GetActorForwardVector() * val);
}

void ACruxCharacter::MoveRight(float val)
{
	AddMovementInput(GetActorRightVector() * val);
}

void ACruxCharacter::BeginCrouch()
{
	Crouch();
}

void ACruxCharacter::EndCrouch()
{
	UnCrouch();
}

void ACruxCharacter::RightMousePressed()
{
	bUseControllerRotationYaw = true;
	BeginRotation();
}

void ACruxCharacter::RightMouseReleased()
{
	
	ACruxPlayerController* controller = Cast<ACruxPlayerController>(GetController());
	if (controller)
	{
		FHitResult hit_result;
		controller->GetHitResultUnderCursor(COLLISION_CLICKABLE, true, hit_result);	
		ACruxCharacter* hit_actor = Cast<ACruxCharacter>(hit_result.GetActor());
		Target = hit_actor;
		Targeted(hit_actor);
	}

	bUseControllerRotationYaw = false;
	EndRotation();
}

void ACruxCharacter::LeftMousePressed()
{
	BeginRotation();
}

void ACruxCharacter::LeftMouseReleased()
{
	EndRotation();
}

void ACruxCharacter::BeginRotation()
{
	if (IsRotating)
	{
		return;
	}

	IsRotating = true;

	ACruxPlayerController* controller = Cast<ACruxPlayerController>(GetController());
	if (controller)
	{
		controller->GetMousePosition(MouseX, MouseY);
		controller->SetIgnoreLookInput(false);
		controller->bShowMouseCursor = false;
	}
}

void ACruxCharacter::EndRotation()
{
	ACruxPlayerController* controller = Cast<ACruxPlayerController>(GetController());
	if (controller && IsRotating &&
		!controller->IsInputKeyDown(EKeys::RightMouseButton) && 
		!controller->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		controller->SetMouseLocation(MouseX, MouseY);
		controller->SetIgnoreLookInput(true);
		controller->bShowMouseCursor = true;

		IsRotating = false;
	}
}

void ACruxCharacter::OnHealthChanged(UCruxHealthComponent* Comp, float Health, 
	float HealthDelta, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCause)
{
	if (Health <= 0.0f && !IsDead)
	{
		IsDead = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

FVector ACruxCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

void ACruxCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACruxCharacter, IsDead);
	DOREPLIFETIME(ACruxCharacter, IsAttacking);
	DOREPLIFETIME(ACruxCharacter, ActorName);
}