// Fill out your copyright notice in the Description page of Project Settings.


#include "CruxAbilityMeleeAction.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "ProjectCrux.h"

void UCruxAbilityMeleeAction::MultiSphereTraceStrike()
{
	TArray<FHitResult> HitResults;

	FVector ReachVector = ActionOwner->GetActorForwardVector() * Reach;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ActionOwner);
	QueryParams.bTraceComplex = true;
	bool BlockingHit = GetWorld()->SweepMultiByChannel(
		HitResults, StartLocation, StartLocation + ReachVector, Rotation,
		COLLISION_ATTACKS, CollisionShape, QueryParam);
	
	if (BlockingHit)
	{
		TArray<AActor> HitActors;
		for (FHitResult Result : HitResults)
		{
			if (Result->Actor)
			{
				HitActors->AddUnique(Result->Actor);
			}
		}

		for (AActor *Actor : HitActors)
		{
			UGameplayStatics::ApplyDamage(Actor, Damage, Actor->GetInstigatorController(), ActionOwner);
		}
	}
}
