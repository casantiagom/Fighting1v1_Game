// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTING_API AAI_Controller : public AAIController
{
	GENERATED_BODY()


public:
	void BeginPlay() override;

private:

	class UNavigationSystemV1* NavArea;

	FVector RandomLocation;

public:

	UFUNCTION()
		void RandomPatrol();
	
};
