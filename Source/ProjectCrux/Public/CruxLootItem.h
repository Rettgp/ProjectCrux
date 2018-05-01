// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CruxLootItem.generated.h"

UCLASS()
class PROJECTCRUX_API ACruxLootItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACruxLootItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "LootItem", meta = (ExposeOnSpawn = "true"))
	int Item;
};
