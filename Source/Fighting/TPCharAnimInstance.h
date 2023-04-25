// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTING_API UTPCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UTPCharAnimInstance();

protected:
	virtual void NativeUninitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
	class AFightingCharacter* PlayerReference;



	
};
