// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxCharacterMovementComponent.h"

#include "GameFramework/Character.h"

UCruxCharacterMovementComponent::UCruxCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCruxCharacterMovementComponent::SetMoveSpeed(float Speed)
{
	CurrentMoveSpeed = Speed;
	UseCustomMoveSpeed = true;
}

void UCruxCharacterMovementComponent::ResetMoveSpeed()
{
	CurrentMoveSpeed = Super::GetMaxSpeed();

	UseCustomMoveSpeed = false;
}

void UCruxCharacterMovementComponent::Dash(FVector Direction)
{
	//Send movement vector to server
	if (PawnOwner->GetLocalRole() < ROLE_Authority)
	{
		ServerSetDashDirection(Direction);
	}

	DashVector = Direction;
	UseDash = true;
}

float UCruxCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (UseCustomMoveSpeed)
	{
		MaxSpeed = CurrentMoveSpeed;
	}

	return MaxSpeed;
}

float UCruxCharacterMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();
	return MaxAccel;
}

//Set input flags on character from saved inputs
void UCruxCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	UseCustomMoveSpeed = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* UCruxCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UCruxCharacterMovementComponent* MutableThis = const_cast<UCruxCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MyMovement(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

bool UCruxCharacterMovementComponent::ServerSetDashDirection_Validate(const FVector& DashDir)
{
	return true;
}

void UCruxCharacterMovementComponent::ServerSetDashDirection_Implementation(const FVector& DashDir)
{
	DashVector = DashDir;
}

void UCruxCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	//Update dodge movement
	if (UseDash)
	{		
		FVector DodgeVel = DashVector;
		DodgeVel.Z = 0.0f;

		Launch(DodgeVel);

		UseDash = false;
	}
}

void FSavedMove_MyMovement::Clear()
{
	Super::Clear();

	SavedUseCustomMoveSpeed = false;
	SavedDashDirection = FVector::ZeroVector;
}

uint8 FSavedMove_MyMovement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedUseCustomMoveSpeed)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool FSavedMove_MyMovement::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//This pretty much just tells the engine if it can optimize by combining saved moves. There doesn't appear to be
	//any problem with leaving it out, but it seems that it's good practice to implement this anyways.
	if (SavedUseCustomMoveSpeed != ((FSavedMove_MyMovement*)&NewMove)->SavedUseCustomMoveSpeed)
	{
		return false;
	}
	if (SavedDashDirection != ((FSavedMove_MyMovement*)&NewMove)->SavedDashDirection)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MyMovement::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCruxCharacterMovementComponent* CharMov = Cast<UCruxCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		SavedUseCustomMoveSpeed = CharMov->UseCustomMoveSpeed;

		//Again, just taking the player movement component's state and storing it for later it in the saved move.
		SavedDashDirection = CharMov->DashVector;
	}
}

void FSavedMove_MyMovement::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCruxCharacterMovementComponent* CharMov = Cast<UCruxCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		//This is just the exact opposite of SetMoveFor. It copies the state from the saved move to the movement
		//component before a correction is made to a client.
		CharMov->DashVector = SavedDashDirection;
		
		//Don't update flags here. They're automatically setup before corrections using the compressed flag methods.	
	}
}

FNetworkPredictionData_Client_MyMovement::FNetworkPredictionData_Client_MyMovement(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_MyMovement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MyMovement());
}
