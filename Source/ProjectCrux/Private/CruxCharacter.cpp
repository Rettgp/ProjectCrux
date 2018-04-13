// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CruxHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"


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
}