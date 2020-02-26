// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CruxAbilityAction.h"
#include "CruxAbilityMeleeAction.generated.h"

/**
 * 
 */
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityMeleeAction : public UCruxAbilityAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMeleeAction")
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMeleeAction")
	float Reach;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMeleeAction")
	float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMeleeAction")
	float AttackDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMeleeAction")
	float Damage;
};
