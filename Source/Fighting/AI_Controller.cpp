// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "NavigationSystem.h"

void AAI_Controller::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	RandomPatrol();
}

void AAI_Controller::RandomPatrol()
{
	if (NavArea)
	{
		NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
			RandomLocation, 15000.0f);

		MoveToLocation(RandomLocation);
	}
}