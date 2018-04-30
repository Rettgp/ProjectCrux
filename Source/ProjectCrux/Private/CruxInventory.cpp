// Fill out your copyright notice in the Description page of Project Settings.

#include "CruxInventory.h"

UCruxInventory::UCruxInventory()
{
	Items.Push(0);
	Items.Push(1);
	Items.Push(2);
}

UCruxInventory::~UCruxInventory()
{
}

TArray<int> UCruxInventory::GetItems() const
{
	return Items;
}

void UCruxInventory::AddItem(int item)
{
	Items.Push(item);
}

void UCruxInventory::RemoveItem(int item)
{
	Items.Remove(item);
}
