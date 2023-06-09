// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enemy_AnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTING_API UEnemy_AnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	bool TakeHit;
};
