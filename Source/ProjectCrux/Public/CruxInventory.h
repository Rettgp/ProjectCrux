// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CruxInventory.generated.h"

UCLASS()
class PROJECTCRUX_API UCruxInventory : public UObject
{
	GENERATED_BODY()

public:
	UCruxInventory();
	~UCruxInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<int> GetItems() const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(int item);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(int item);

private:
	TArray<int> Items;
};
