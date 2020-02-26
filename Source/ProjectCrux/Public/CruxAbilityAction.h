// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CruxAbilityAction.generated.h"

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityAction : public UObject
{
	GENERATED_BODY()
public:
	UCruxAbilityAction();
	~UCruxAbilityAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AbilityAction")
	void Run(AActor *owner);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AbilityAction")
	void Tick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "AbilityAction")
	void OnCompleted();
	UFUNCTION(BlueprintCallable, Category = "AbilityAction")
	bool IsComplete();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	bool AffectFriendly;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	bool AffectEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityAction")
	bool Completed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityAction")
	float ActionTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityAction")
	UAnimSequence *ActionAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityAction")
	AActor *Owner;

};
