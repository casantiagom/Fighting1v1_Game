// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharAnimInstance.h"
#include "FightingCharacter.h"


UTPCharAnimInstance::UTPCharAnimInstance()
{

	
}

void UTPCharAnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
	PlayerReference = Cast<AFightingCharacter>(TryGetPawnOwner());
}

void UTPCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
