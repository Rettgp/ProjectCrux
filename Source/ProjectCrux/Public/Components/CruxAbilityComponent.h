// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CruxAbilityAction.h"
#include "CruxAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownUpdatedSignature, UCruxAbilityComponent*, AbilityComp, float, CooldownRemaining, class AController*, InstigatedBy );


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCruxAbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_CooldownRemaining, BlueprintReadOnly, Category = "AbilityComponent")
	float CooldownRemaining;
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityComponent")
	TArray<TSubclassOf<UCruxAbilityAction>> Actions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityComponent")
	bool Casting;
	
	UFUNCTION(BlueprintCallable, Category = "AbilityComponent")
	bool CanCast();
	UFUNCTION()
	void OnRep_CooldownRemaining(float old_cooldown);
	UFUNCTION(BlueprintCallable, Category = "AbilityComponent")
	void Cast();
	UFUNCTION(Server, Reliable)
	void ServerCast();

	int ActionIndex;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCooldownUpdatedSignature OnCooldownUpdated;
};
