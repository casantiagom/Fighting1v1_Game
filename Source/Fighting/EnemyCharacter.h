// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AkAudioEvent.h"
#include "InputActionValue.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class FIGHTING_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaSeconds) override;

protected:

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth = 100;

	
	
	
	void MainAttack();
	
	
	
	UNiagaraComponent* Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	
	void Move(const FInputActionValue& Value);


	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	virtual void BeginPlay();

public:
	AEnemyCharacter();

	float GetHealth() const { return Health; }
	void SetHealth( float val ) { Health = val; }

	float GetDefaultHealth() const { return DefaultHealth; }
	void SetDefaultHealth( float val ) { DefaultHealth = val; }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	TArray<AActor*> OverlappingActors;
	TSubclassOf<AActor> FilterActor = nullptr;
	FVector MutantBoxCollisionVector;
	bool PlayerDetected;
	bool CanAttackPlayer;
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackCollision;

	class UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UAnimNotify* EnemyPunch_NS;
	
	class AAI_Controller* EnemyAIController;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere)
	float StopDistance = 100.0f;

	FTimerHandle SeekPlayerTimerHandle;

	void Punch();
	void BigPunch();
	void StopBigPunch();
	void JumpPunch();
	void StopJumpPunch();
	void RoarBlueFire();
	void StopRoarBlueFire();
	void Grab();
	void Death();
	void EnableWalk();
	void PlayPlayerEndGame();
	

	void DealDamageTo(AActor* OtherActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool IsGettingHit;

	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;
	
	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);
	
	class AFightingCharacter* PlayerRef;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision2")
	UBoxComponent* HitBox2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision3")
	UBoxComponent* HitBox3;
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool IsPunching;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasBigPunched;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasDied;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasRoarBlueFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Bool, meta = (AllowPrivateAccess = "true"))
	bool HasJumpPunched;

	
	USkeletalMeshComponent* CharMesh;

	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* NiagaraFX;
	
	FName HeadSocket;

	UAkAudioEvent* Z_TakeHit;
	UAkAudioEvent* Z_Footstep;
	UAkAudioEvent* Z_Jump;
	UAkAudioEvent* Z_Land;
	UAkAudioEvent* Z_Punch;
	UAkAudioEvent* Z_BigPunch;
	UAkAudioEvent* Z_Death;

	void StopPunch();

	UFUNCTION()
		void MoveToPlayer();

	UFUNCTION()
		void SeekPlayer();

	UFUNCTION()
		void StopSeekingPlayer();

	UFUNCTION()
		void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDealDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private:

	FTimerHandle TimerMovementWalking;
	UCharacterMovementComponent* MoveCompRef = nullptr;
	// Save pointer reference for CharacterMovementComponent
	
	
	
	
};
