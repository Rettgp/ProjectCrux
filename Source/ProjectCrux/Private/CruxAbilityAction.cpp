// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxAbilityAction.h"

UCruxAbilityAction::UCruxAbilityAction()
{
	AffectEnemy = true;
	AffectFriendly = false;
	ActionTime = 1.0;
	Completed = false;
}

UCruxAbilityAction::~UCruxAbilityAction()
{
}

bool UCruxAbilityAction::IsComplete()
{
	return Completed;
}
