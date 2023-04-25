// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AkGameplayStatics.h"
#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MutantAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class FIGHTING_API UMutantAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
		


public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAkAudioEvent* Event;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAkAudioEvent* Event2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bIsEnd;


	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;		

};
