// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CruxAbilityAction.h"
#include "CruxAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCooldownUpdatedSignature, UCruxAbilityComponent*, AbilityComp, float, CooldownRemaining, int, Slot, class AController*, InstigatedBy );


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCruxAbilityComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityComponent")
	int Slot;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_CooldownRemaining, BlueprintReadOnly, Category = "AbilityComponent")
	float CooldownRemaining;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityComponent")
	float Cooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityComponent")
	TArray<TSubclassOf<UCruxAbilityAction>> Actions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityComponent")
	bool Casting;
	
	UFUNCTION(BlueprintCallable, Category = "AbilityComponent")
	bool CanCastAbility();
	UFUNCTION()
	void OnRep_CooldownRemaining(float old_cooldown);
	UFUNCTION(BlueprintCallable, Category = "AbilityComponent")
	void CastAbility();
	UFUNCTION(BlueprintNativeEvent, Category = "AbilityComponent")
	void CooldownFinished();
	UFUNCTION(Server, Reliable)
	void ServerCastAbility();

	int ActionIndex;
	FTimerHandle CooldownTimerHandle;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCooldownUpdatedSignature OnCooldownUpdated;
};
