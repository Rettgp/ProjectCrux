// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxAbilityComponent.h"

#include "Net/UnrealNetwork.h"

#include "CruxCharacter.h"

// Sets default values for this component's properties
UCruxAbilityComponent::UCruxAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UCruxAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	CooldownRemaining = 0.0f;
	Cooldown = 1.0f;
	ActionIndex = 0;
	Casting = false;
}


bool UCruxAbilityComponent::CanCastAbility()
{
	return (CooldownRemaining <= 0.0f);
}

void UCruxAbilityComponent::OnRep_CooldownRemaining(float old_cooldown)
{
	OnCooldownUpdated.Broadcast(this, CooldownRemaining, Slot, nullptr);
}

void UCruxAbilityComponent::CastAbility()
{
	if (!CanCastAbility() || Actions.Num() <= 0)
	{
		return;
	}

	ActionIndex = 0;
	UCruxAbilityAction *action = Actions[ActionIndex]->GetDefaultObject<UCruxAbilityAction>();
	ACruxCharacter *character = Cast<ACruxCharacter>(GetOwner());
	if (character)
	{
		action->Run(character);
		Casting = true;

		ServerCastAbility();
	}
}

void UCruxAbilityComponent::CooldownFinished_Implementation()
{
}

void UCruxAbilityComponent::ServerCastAbility_Implementation()
{
	if (!CanCastAbility() || Actions.Num() <= 0)
	{
		return;
	}

	ActionIndex = 0;
	UCruxAbilityAction *action = Actions[ActionIndex]->GetDefaultObject<UCruxAbilityAction>();
	ACruxCharacter *character = Cast<ACruxCharacter>(GetOwner());
	if (character)
	{
		action->Run(character);
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UCruxAbilityComponent::CooldownFinished, Cooldown, false);
		Casting = true;
	}
}

// Called every frame
void UCruxAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CooldownRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
	UE_LOG(LogTemp, Warning, TEXT("%f"), CooldownRemaining)
	OnCooldownUpdated.Broadcast(this, CooldownRemaining, Slot, GetOwner()->GetInstigatorController());

	if (Casting)
	{
		UCruxAbilityAction *ActiveAction = Actions[ActionIndex]->GetDefaultObject<UCruxAbilityAction>();
		ActiveAction->Tick(DeltaTime);

		if (ActiveAction->IsComplete())
		{
			ActionIndex++;
			if (ActionIndex >= Actions.Num())
			{
				Casting = false;
				ActionIndex = 0;
			}
			else
			{
				UCruxAbilityAction *NextAction = Actions[ActionIndex]->GetDefaultObject<UCruxAbilityAction>();
				ACruxCharacter *character = Cast<ACruxCharacter>(GetOwner());
				if (character)
				{
					NextAction->Run(character);
				}
			}
		}
	}
}

void UCruxAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCruxAbilityComponent, CooldownRemaining);
}

