// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CruxCharacter.h"

#include "CruxAbilityAction.generated.h"

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityAction : public UObject
{
	GENERATED_BODY()
public:
	UCruxAbilityAction();
	~UCruxAbilityAction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbilityAction")
	void Run(ACruxCharacter *Owner);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AbilityAction")
	void Tick(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "AbilityAction")
	bool IsComplete();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbilityAction")
	void OnCompleted();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AbilityAction")
	void Interrupt();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	bool AffectFriendly;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	bool AffectEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityAction")
	float ActionTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	UAnimSequence *ActionAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	ACruxCharacter *ActionOwner;

	FTimerHandle ActionTimerHandle;
	bool Completed;
};
