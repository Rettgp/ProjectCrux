// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxAbilityAction.h"

UCruxAbilityAction::UCruxAbilityAction()
{
	AffectEnemy = true;
	AffectFriendly = false;
	ActionTime = 1.0;
}

UCruxAbilityAction::~UCruxAbilityAction()
{
}

void UCruxAbilityAction::Run_Implementation(ACruxCharacter *Owner)
{
	ActionOwner = Owner;
	Completed = false;
	ActionOwner->GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &UCruxAbilityAction::OnCompleted, ActionTime, false);
}

void UCruxAbilityAction::OnCompleted_Implementation()
{
	Completed = true;
}

bool UCruxAbilityAction::IsComplete()
{
	return Completed;
}

void UCruxAbilityAction::Interrupt_Implementation()
{
	if (ActionOwner)
	{
		ActionOwner->GetWorld()->GetTimerManager().ClearTimer(ActionTimerHandle);
	}
	Completed = true;
}
