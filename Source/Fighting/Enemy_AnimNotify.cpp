// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AnimNotify.h"

#include "EnemyCharacter.h"

void UEnemy_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	AEnemyCharacter	* EnemyRef = Cast<AEnemyCharacter>(MeshComp->GetOwner());
	
	if (MeshComp && Animation && EnemyRef)
	{
		if (EnemyRef)
		{
			FOnAkPostEventCallback nullCallback;
			
			
			if (Animation->GetName() == FString("Punching"))
			{
				EnemyRef->Z_Punch->PostOnActor(EnemyRef, nullCallback,int32(0), true);
			}

			
			if (Animation->GetName() == FString("Hook"))
			{
				EnemyRef->Z_BigPunch->PostOnActor(EnemyRef, nullCallback,int32(0), true);
			}
			
			
			if (Animation->GetName() == FString("Head_Hit") && TakeHit)
			{
				EnemyRef->Z_TakeHit->PostOnActor(EnemyRef, nullCallback,int32(0), true);
			}
			
			
			if (Animation->GetName() == FString("Standing_Death_Left_01"))
			{
				EnemyRef->Z_Death->PostOnActor(EnemyRef, nullCallback,int32(0), true);
				
			}

			if (Animation->GetName() == FString("Walking"))
			{
				EnemyRef->Z_Footstep->PostOnActor(EnemyRef, nullCallback,int32(0), true);
			}

			if (Animation->GetName() == FString("Running"))
			{
				EnemyRef->Z_Footstep->PostOnActor(EnemyRef, nullCallback,int32(0), true);
			}
			
		}
	}
}
