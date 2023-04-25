// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTING_API UEnemy_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemy_AnimInstance();

protected:
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class AEnemyCharacter* EnemyReference;



	
};
