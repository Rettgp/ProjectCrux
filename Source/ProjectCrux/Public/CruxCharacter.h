// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CruxCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCruxHealthComponent;
class ACruxWeapon;

UENUM(BlueprintType)
enum class EAttackSide : uint8
{
	LEFT,
	RIGHT
};

USTRUCT(BlueprintType)
struct FAutoAttackInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Auto Attack Info")
	EAttackSide AutoAttackSide;
};

UCLASS()
class PROJECTCRUX_API ACruxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACruxCharacter();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AutoAttackHit(FAutoAttackInfo AttackInfo);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAutoAttackHit(FAutoAttackInfo AttackInfo);

	virtual float AutoAttackRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float val);
	void MoveRight(float val);
	void BeginCrouch();
	void EndCrouch();
	void BeginAutoAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTarget(ACruxCharacter* target);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginAutoAttack();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void AutoAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void Targeted(AActor* target);
	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void AutoAttackNotify(FAutoAttackInfo AttackInfo);
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
	bool AutoAttackStarted;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	FString ActorName;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	AActor* Target;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Character")
	ACruxWeapon* CurrentWeapon;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetPawnViewLocation() const override;
};
