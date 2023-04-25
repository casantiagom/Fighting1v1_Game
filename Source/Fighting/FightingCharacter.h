// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AkGameplayStatics.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/HUD.h"
#include "FightingCharacter.generated.h"


UCLASS(config=Game)
class AFightingCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PunchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BigPunchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RoarBlueFireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpPunchAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UAnimNotify* PunchStopNotify;
	
public:
	AFightingCharacter();

	int32 PlayingId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UAkAudioEvent* M_Fire_Loop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UAkAudioEvent* M_Fire_Loop_End;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UAkAudioEvent* M_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
	UAkAudioEvent* Arrive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	bool bIsActivated = false;

	
	
	UAkAudioEvent* Song;
	UAkAudioEvent* SongStop;
	UAkAudioEvent* EndSong;
	UAkAudioEvent* M_BigPunchAudioEvent;
	UAkAudioEvent* JumpAudioEvent;

	UCharacterMovementComponent* MoveCompRef = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "damage")
	UBoxComponent* DamageBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision2")
	UBoxComponent* HitBox2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "damage2")
	UBoxComponent* DamageBox2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool IsPunching = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasBigPunched;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasDied;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasRoarBlueFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasJumpPunched;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool IsGettingHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* NiagaraFX;


	void PlayEndGame();
	void DealDamageTo(class AActor* OtherActor, float DamageAmount);
	virtual float TakeDamage
	(float damageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	FName HeadSocket;

	bool CanAttackEnemy;
	bool CanDealDamage = false;
	class AEnemyCharacter* EnemyRef;

	void StopPunch();
	void EnableWalk();
	void BigPunch();
	void StopBigPunch();
	
	void RoarBlueFire();
	void StopRoarBlueFire();

	void Landed(const FHitResult& Hit) override;
	void Jump() override;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* EnemyCollision;

	UPROPERTY(EditAnywhere)
	USphereComponent* EnemyAttackCollision;

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDealDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth = 100;

	UNiagaraComponent* Effect;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Punch();
	void Grab();
	void Death();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay();
};

