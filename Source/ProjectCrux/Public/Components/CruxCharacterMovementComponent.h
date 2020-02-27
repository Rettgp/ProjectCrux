// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CruxCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCRUX_API UCruxCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

public:
	friend class FSavedMove_MyMovement;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable, Category = "CruxMovement")
	void SetMoveSpeed(float Speed);
	UFUNCTION(BlueprintCallable, Category = "CruxMovement")
	void ResetMoveSpeed();
	UFUNCTION(BlueprintCallable, Category = "CruxMovement")
	void Dash(FVector Direction);

	UFUNCTION(Unreliable, Server, WithValidation)
	void ServerSetDashDirection(const FVector& DashDir);

	///@brief Event triggered at the end of a movement update
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity) override;

	virtual float GetMaxSpeed() const;
	virtual float GetMaxAcceleration() const;
	uint8 UseCustomMoveSpeed : 1;
	uint8 UseDash : 1;

protected:
	float CurrentMoveSpeed;
	FVector DashVector;
};

class FSavedMove_MyMovement : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;

	///@brief Resets all saved variables.
	virtual void Clear() override;
	///@brief Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;
	///@brief This is used to check whether or not two moves can be combined into one.
	///Basically you just check to make sure that the saved variables are the same.
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
	///@brief Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
	///@brief Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* Character) override;

	FVector SavedDashDirection;
	uint8 SavedUseCustomMoveSpeed : 1;
	uint8 SavedUseDash : 1;
};

class FNetworkPredictionData_Client_MyMovement : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_MyMovement(const UCharacterMovementComponent& ClientMovement);
	typedef FNetworkPredictionData_Client_Character Super;
	///@brief Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};
