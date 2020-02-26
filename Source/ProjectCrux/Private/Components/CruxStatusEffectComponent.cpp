// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxStatusEffectComponent.h"

#include "Net/UnrealNetwork.h"

#include "Components/CruxHealthComponent.h"

// Sets default values for this component's properties
UCruxStatusEffectComponent::UCruxStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxLifetime = 1.0;
	Damage = 10.0;
	EffectTickTime = 1.0;

	SetIsReplicated(true);
}

// Called when the game starts
void UCruxStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentLifetime = MaxLifetime;

	GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &UCruxStatusEffectComponent::ServerEffectTick, EffectTickTime, true);
}

// Called every frame
void UCruxStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCruxStatusEffectComponent::ServerEffectTick_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("TICK"));

	CurrentLifetime -= EffectTickTime;

	AActor *owner = GetOwner();
	if (!owner) return;

	EffectTick();

	UCruxHealthComponent *health_comp = owner->FindComponentByClass<UCruxHealthComponent>();

	if ((health_comp && health_comp->GetHealth() <= 0.0f) || CurrentLifetime <= 0.0f)
	{
		UE_LOG(LogTemp,Warning,TEXT("MyCharacter's Health is %f"), health_comp->GetHealth() );
		UE_LOG(LogTemp,Warning,TEXT("Lifetime %f"), CurrentLifetime );
		GetWorld()->GetTimerManager().ClearTimer(EffectTimerHandle);
		DestroyComponent(false);
		ConditionalBeginDestroy();
	}
}

void UCruxStatusEffectComponent::OnRep_CurrentLifetime(float lifetime)
{
	if (!Instigator) return;
	OnStatusEffectTick.Broadcast(this, lifetime, Damage, nullptr, Instigator->GetInstigatorController(), GetOwner());
}

void UCruxStatusEffectComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCruxStatusEffectComponent, CurrentLifetime);
}
