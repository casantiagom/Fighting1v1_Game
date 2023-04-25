// Fill out your copyright notice in the Description page of Project Settings.


#include "MutantAnimNotify.h"

#include "AkAudioEvent.h"
#include "FightingCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "AkAudioModule.h"
#include "WwiseSoundEngine/Public/Wwise/WwiseSoundEngineModule.h"
#include "Engine/Engine.h"




void UMutantAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	AFightingCharacter* PlayerRef = Cast<AFightingCharacter>(MeshComp->GetOwner());
	
	if (MeshComp && Animation && PlayerRef)
	{
		if (PlayerRef->bIsActivated)
		{
			FOnAkPostEventCallback nullCallback;
			
			
			if (Event && Event->GetName() == FString("M_Attack"))
			{

				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}

			
			if (Event && Event->GetName() == FString("M_Fire_Loop"))
			{
				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}
			
			
			if (Event && Event->GetName() == FString("M_Fire_Start"))
			{
				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}
			
			
			if (Animation->GetName() == FString("mutant_swiping"))
			{
				PlayerRef->M_BigPunchAudioEvent->PostOnActor(PlayerRef, nullCallback,int32(0), true);

			}

			if (Animation->GetName() == FString("mutant_run"))
			{
				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}

			if (Animation->GetName() == FString("mutant_walking"))
			{
				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}
			

			if (Animation->GetName() == FString("Reaction_Hit"))
			{
				Event->PostOnActor(PlayerRef, nullCallback,int32(0), true);
			}
			
		}
	}
}
