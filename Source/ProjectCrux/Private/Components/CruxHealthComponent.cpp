// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxHealthComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UCruxHealthComponent::UCruxHealthComponent()
{
DefaultHealth = 100.0f;
	IsDead = false;

	TeamId = 255;

	SetIsReplicated(true);
}


// Called when the game starts
void UCruxHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hook if we are the server
	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* owner = GetOwner();
		if (owner)
		{
			owner->OnTakeAnyDamage.AddDynamic(this, &UCruxHealthComponent::HandleTakeAnyDamage);
		}
	}
	Health = DefaultHealth;
}

void UCruxHealthComponent::HandleTakeAnyDamage(
	AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead)
	{
		return;
	}

	if ((DamageCauser != DamagedActor) && 
		IsFriendly(DamagedActor, DamageCauser))
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	IsDead = (Health <= 0.0f);

	if (IsDead)
	{
		// TODO: Handle telling the game mode someone died?
	}

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UCruxHealthComponent::OnRep_Health(float old_health)
{
	float damage = Health - old_health;
	OnHealthChanged.Broadcast(this, Health, damage, nullptr, nullptr, nullptr);
}

float UCruxHealthComponent::GetHealth() const
{
	return Health;
}

void UCruxHealthComponent::Heal(float amount)
{
	if (amount <= 0.0f || IsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health + amount, 0.0f, DefaultHealth);

	OnHealthChanged.Broadcast(this, Health, -amount, nullptr, nullptr, nullptr);
}

bool UCruxHealthComponent::IsFriendly(AActor* actor_a, AActor* actor_b)
{
	if (!actor_a || !actor_b)
	{
		return true;
	}

	auto health_comp_a = Cast<UCruxHealthComponent>(actor_a->GetComponentByClass(UCruxHealthComponent::StaticClass()));
	auto health_comp_b = Cast<UCruxHealthComponent>(actor_b->GetComponentByClass(UCruxHealthComponent::StaticClass()));

	if (!health_comp_a || !health_comp_b)
	{
		return false;
	}

	if (health_comp_a->TeamId == 255 || health_comp_b->TeamId == 255)
	{
		return false;
	}

	return health_comp_a->TeamId == health_comp_b->TeamId;
}

void UCruxHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCruxHealthComponent, Health);
}
