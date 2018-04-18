// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxWeapon.h"


// Sets default values
ACruxWeapon::ACruxWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);

	BaseDamage = 25.0f;
	CriticalRate = 0.01f;
	CriticalMultiplier = 1.5f;
	PrimarySocketName = "Socket";
}

// Called when the game starts or when spawned
void ACruxWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACruxWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACruxWeapon::PrimarySocketLocation() const
{
	return SkeletalMeshComp->GetSocketLocation(PrimarySocketName);
}

