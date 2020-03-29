// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CruxAbilityAction.h"
#include "CruxAbilityMovementAction.generated.h"

/**
 * 
 */
UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCRUX_API UCruxAbilityMovementAction : public UCruxAbilityAction
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "AbilityMovementAction")
	void ToggleGroundFriction(bool EnableFriction);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMovementAction")
	float Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityMovementAction", meta=(ClampMin="0.0", ClampMax="1.0"))
	FVector Direction;
};
