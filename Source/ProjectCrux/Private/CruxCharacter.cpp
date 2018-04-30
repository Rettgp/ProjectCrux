// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CruxHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "CruxPlayerController.h"
#include "ProjectCrux.h"
#include "CruxWeapon.h"
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
	AutoAttackStarted = false;
	ActorName = "Player";
	CurrentWeapon = nullptr;
}

// Called when the game starts or when spawned
void ACruxCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComp->OnHealthChanged.AddDynamic(this, &ACruxCharacter::OnHealthChanged);
	Inventory = NewObject<UCruxInventory>(this);
}

// Called every frame
void ACruxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector eye_location;
	FRotator eye_rotation;
	GetActorEyesViewPoint(eye_location, eye_rotation);

	FVector look_dir = eye_rotation.Vector();

	FCollisionQueryParams query_params;
	query_params.AddIgnoredActor(this);
	query_params.bReturnPhysicalMaterial = false;
	query_params.bTraceComplex = true;

	FVector trace_end = eye_location + (look_dir * 10000);
	FVector tracer_end_point = trace_end;

	EPhysicalSurface surface_type = SurfaceType_Default;
	FHitResult hit_result;
	bool blocking_hit = GetWorld()->LineTraceSingleByChannel(
		hit_result, eye_location, trace_end, COLLISION_HIGHLIGHT, query_params);

	if (blocking_hit)
	{
		Targeted(hit_result.GetActor());
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
	PlayerInputComponent->BindAction("AutoAttack", IE_Pressed, this, &ACruxCharacter::BeginAutoAttack);
}


// ONLY RAN ON SERVER
void ACruxCharacter::AutoAttackHit(FAutoAttackInfo AttackInfo)
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	AutoAttackStarted = false;

	AutoAttackNotify(AttackInfo);
}

float ACruxCharacter::AutoAttackRange()
{
	return 250.0f;
}

void ACruxCharacter::ServerAutoAttackHit_Implementation(FAutoAttackInfo AttackInfo)
{
	AutoAttackHit(AttackInfo);
}

bool ACruxCharacter::ServerAutoAttackHit_Validate(FAutoAttackInfo AttackInfo)
{
	return true;
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

void ACruxCharacter::ServerTarget_Implementation(ACruxCharacter* target)
{
	Target = target;
}

bool ACruxCharacter::ServerTarget_Validate(ACruxCharacter* target)
{
	(void)target;
	return true;
}

void ACruxCharacter::BeginAutoAttack()
{
	if (Role < ROLE_Authority)
	{
		ServerBeginAutoAttack();
	}

	AutoAttackStarted = true;
}

void ACruxCharacter::ServerBeginAutoAttack_Implementation()
{
	BeginAutoAttack();
}

bool ACruxCharacter::ServerBeginAutoAttack_Validate()
{
	return true;
}

void ACruxCharacter::OnHealthChanged(UCruxHealthComponent* Comp, float Health, 
	float HealthDelta, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCause)
{
	if (Health <= 0.0f && !IsDead)
	{
		IsDead = true;

		Die();

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
	DOREPLIFETIME(ACruxCharacter, AutoAttackStarted);
	DOREPLIFETIME(ACruxCharacter, ActorName);
	DOREPLIFETIME(ACruxCharacter, Target);
	DOREPLIFETIME(ACruxCharacter, Inventory);
}