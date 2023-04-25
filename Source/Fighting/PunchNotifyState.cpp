// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchNotifyState.h"
#include "FightingCharacter.h"
#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPunchNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && Animation)
	{
		AFightingCharacter* Player = Cast<AFightingCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			GEngine->AddOnScreenDebugMessage(-1,4.5f, FColor::Purple, Animation->GetName());
			Player->GetCharacterMovement()->DisableMovement();
		}
	}
}

void UPunchNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime )
{
	
}

void UPunchNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && Animation)
	{
		AFightingCharacter* Player = Cast<AFightingCharacter>(MeshComp->GetOwner());
		if (Player)
		{
			if (Animation->GetName() == FString("Reaction_Hit"))
			{
				GEngine->AddOnScreenDebugMessage(-1,4.5f, FColor::White, TEXT("Mutang getting HIT"));
				Player->IsGettingHit = false;
				Player->EnableWalk();
			}
			
			if (Animation->GetName() == FString("mutant_punch"))
			{
				Player->StopPunch();
				Player->EnableWalk();
				
			}

			if (Animation->GetName() == FString("mutant_swiping"))
			{
				Player->StopBigPunch();
				Player->EnableWalk();
				
			}
			
			if (Animation->GetName() == FString("mutant_idle__2_"))
			{
				Player->StopRoarBlueFire();
				Player->EnableWalk();
			}
		}
	}
}
