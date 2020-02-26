// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxCharacterMovementComponent.h"

#include "GameFramework/Character.h"

UCruxCharacterMovementComponent::UCruxCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCruxCharacterMovementComponent::SetMoveSpeed(float CustomVelocity)
{
	CurrentMoveSpeed = CustomVelocity;
	UseCustomMoveSpeed = true;
}

void UCruxCharacterMovementComponent::ResetMoveSpeed()
{
	CurrentMoveSpeed = Super::GetMaxSpeed();

	UseCustomMoveSpeed = false;
}

float UCruxCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	if (UseCustomMoveSpeed)
	{
		MaxSpeed = CurrentMoveSpeed;
	}

	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), MaxSpeed);
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

void FSavedMove_MyMovement::Clear()
{
	Super::Clear();

	SavedUseCustomMoveSpeed = false;
}

uint8 FSavedMove_MyMovement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedUseCustomMoveSpeed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Compressed flags using custom move speed"));
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
		UE_LOG(LogTemp, Warning, TEXT("CANNOT COMBINE WITH"));
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
	}
}

void FSavedMove_MyMovement::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCruxCharacterMovementComponent* CharMov = Cast<UCruxCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		
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
