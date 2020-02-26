// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CruxStatusEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnStatusEffectTickSignature, UCruxStatusEffectComponent*, StatusEffectComp, float, Lifetime, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCause);


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCruxStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentLifetime, BlueprintReadOnly, Category = "StatusEffectComponent")
	float CurrentLifetime; // s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectComponent")
	float MaxLifetime; // s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectComponent")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectComponent")
	float EffectTickTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectComponent")
	AActor *Instigator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffectComponent")
	UParticleSystem *StatusEffectFx;

	UFUNCTION(BlueprintImplementableEvent, Category = "StatusEffectComponent")
	void EffectTick();
	UFUNCTION(Server, Reliable)
	void ServerEffectTick();
	UFUNCTION()
	void OnRep_CurrentLifetime(float lifetime);

	FTimerHandle EffectTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatusEffectTickSignature OnStatusEffectTick;
		
};
