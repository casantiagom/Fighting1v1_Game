// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AnimInstance.h"
#include "EnemyCharacter.h"


UEnemy_AnimInstance::UEnemy_AnimInstance()
{

	
}

void UEnemy_AnimInstance::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
	EnemyReference = Cast<AEnemyCharacter>(TryGetPawnOwner());
}

void UEnemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
