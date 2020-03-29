// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxAbilityMovementAction.h"

#include "Components/CruxCharacterMovementComponent.h"

void UCruxAbilityMovementAction::ToggleGroundFriction(bool EnableFriction)
{
	if (ActionOwner)
	{
		UCruxCharacterMovementComponent *Movement = 
			Cast<UCruxCharacterMovementComponent>(ActionOwner->GetCharacterMovement());

		if (Movement)
		{
			Movement->ToggleGroundFriction(EnableFriction);
		}
	}
}
