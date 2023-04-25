// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPunch_NS.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyPunch_NS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && Animation)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (Enemy)
		{
			if (Animation->GetName() == FString("Head_Hit"))
			{
				Enemy->GetCharacterMovement()->DisableMovement();
			}
		}
	}
}

void UEnemyPunch_NS::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	
}

void UEnemyPunch_NS::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && Animation)
	{
		GEngine->AddOnScreenDebugMessage(-1,4.5f, FColor::White, Animation->GetName());
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(MeshComp->GetOwner());
		if (Enemy)
		{
			Enemy->EnableWalk();

			if (Animation->GetName() == FString("mutant_jump_attack"))
			{
				Enemy->StopJumpPunch();
			}

			if (Animation->GetName() == FString("Punching"))
			{
				Enemy->StopPunch();
			}
			
			if (Animation->GetName() == FString("Head_Hit"))
			{
				Enemy->IsGettingHit = false;
				Enemy->EnableWalk();
			}

			if (Animation->GetName() == FString("Hook"))
			{
				Enemy->StopBigPunch();
			}
		}
	}
}


