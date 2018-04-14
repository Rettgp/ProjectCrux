// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CruxCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCruxHealthComponent;

UCLASS()
class PROJECTCRUX_API ACruxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACruxCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float val);
	void MoveRight(float val);
	void BeginCrouch();
	void EndCrouch();
	void RightMousePressed();
	void RightMouseReleased();
	void LeftMousePressed();
	void LeftMouseReleased();
	void BeginRotation();
	void EndRotation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void AutoAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void Targeted(ACruxCharacter* target);
	UFUNCTION()
	void OnHealthChanged(UCruxHealthComponent* Comp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCause);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCruxHealthComponent* HealthComp;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character")
	bool IsDead;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	bool IsAttacking;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	FString ActorName;
	UPROPERTY(BlueprintReadWrite, Category = "Character")
	ACruxCharacter* Target;


	bool IsRotating;
	float MouseX;
	float MouseY;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetPawnViewLocation() const override;
};
