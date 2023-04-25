// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "FightingCharacter.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"	
#include "Components/SphereComponent.h"
#include "AI_Controller.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Engine/DamageEvents.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	Health = DefaultHealth;

	
	HitBox2 = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox2"));
	HitBox2->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HitBox2->GetOverlappingActors(OverlappingActors, TSubclassOf< AEnemyCharacter >());

	for (auto actor : OverlappingActors)
	{
		if (OverlappingActors.Num() > 1)
		{
			if (!this)
			{
				return;
			}
		}
	}


	EnemyPunch_NS = Cast<UAnimNotify>(GetMesh()->GetAnimInstance());
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	IsPunching = false;

	
	
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//class USkeletalMesh SkeletalMesh;
	


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	if (MeshComponent)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetRelativeLocation(FVector(40, -25, 100));
		MeshComponent->SetupAttachment(RootComponent);
	}


	

	HeadSocket = TEXT("HeadSocket");

	PlayerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));

	PlayerCollision->SetupAttachment(RootComponent);
	
	PlayerAttackCollision =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));

	PlayerAttackCollision->SetupAttachment(RootComponent);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageBox->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_Jump(TEXT("/Game/WwiseAudio/Jump.Jump"));
	Z_Jump = FindZ_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_TakeHit(TEXT("/Game/WwiseAudio/Z_TakeHit.Z_TakeHit"));
	Z_TakeHit = FindZ_TakeHit.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_Land(TEXT("/Game/WwiseAudio/Arrive.Arrive"));
	Z_Land = FindZ_Land.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_Footstep(TEXT("/Game/WwiseAudio/Footstep.Footstep"));
	Z_Footstep = FindZ_Footstep.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_BigPunch(TEXT("/Game/WwiseAudio/Z_BigPunch.Z_BigPunch"));
	Z_BigPunch = FindZ_BigPunch.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_Punch(TEXT("/Game/WwiseAudio/Z_Punch.Z_Punch"));
	Z_Punch = FindZ_Punch.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindZ_Death(TEXT("/Game/WwiseAudio/Z_Death.Z_Death"));
	Z_Death = FindZ_Death.Object;
	
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveCompRef = GetCharacterMovement();
	
	EnemyAIController = Cast<AAI_Controller>(GetController());
	
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
	(this, &AEnemyCharacter::OnAIMoveCompleted);

	
	
	PlayerCollision->OnComponentBeginOverlap.AddDynamic(this,
	&AEnemyCharacter::OnPlayerDetectedOverlapBegin);

	PlayerCollision->OnComponentEndOverlap.AddDynamic(this,
		&AEnemyCharacter::OnPlayerDetectedOverlapEnd);

	PlayerAttackCollision->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemyCharacter::OnPlayerAttackOverlapBegin);

	PlayerAttackCollision->OnComponentEndOverlap.AddDynamic(this,
		&AEnemyCharacter::OnPlayerAttackOverlapEnd);

	DamageBox->OnComponentBeginOverlap.AddDynamic(this,
		&AEnemyCharacter::OnDealDamageOverlapBegin);

	DamageBox->OnComponentEndOverlap.AddDynamic(this,
		&AEnemyCharacter::OnDealDamageOverlapEnd);

	AnimInstance = GetMesh()->GetAnimInstance();
	
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);




	
}



void AEnemyCharacter::Move(const FInputActionValue& Value)
{
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, 0, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Punch()
{
	if (!IsPunching)
	{
		IsPunching = true;
		UAnimInstance* PunchAnimInstance = GetMesh()->GetAnimInstance();
		
		if (PunchAnimInstance)
		{
			if (!PunchAnimInstance->Montage_IsPlaying(NULL))
			{
				MoveCompRef->DisableMovement();
			}
			else
				{
					EnableWalk();
				}
			
				float EnableWalkTime = PunchAnimInstance->Montage_Play(NULL);
				GetWorldTimerManager().SetTimer(TimerMovementWalking, this, &AEnemyCharacter::EnableWalk, EnableWalkTime);
			}
		}
	}

void AEnemyCharacter::EnableWalk()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->SetMovementMode(MOVE_Walking);
}

void AEnemyCharacter::PlayPlayerEndGame()
{
		PlayerRef->PlayEndGame();
}


void AEnemyCharacter::BigPunch()
{
	
	if (!HasBigPunched)
	{
		HasBigPunched = true;
		UAnimInstance* PunchAnimInstance = GetMesh()->GetAnimInstance();
		
		if (PunchAnimInstance)
		{
			if (!PunchAnimInstance->Montage_IsPlaying(NULL))
			{
				MoveCompRef->DisableMovement();
			}
			else
			{
				EnableWalk();
			}
			
			//float EnableWalkTime = PunchAnimInstance->Montage_Play(NULL);
		//	GetWorldTimerManager().SetTimer(TimerMovementWalking, this, &AEnemyCharacter::EnableWalk, EnableWalkTime);
		}
	}
}

void AEnemyCharacter::JumpPunch()
{
	HasJumpPunched = true;
}

void AEnemyCharacter::StopJumpPunch()
{
	HasJumpPunched = false;
}

void AEnemyCharacter::RoarBlueFire()
{
	HasRoarBlueFire = true;
	Effect->Activate();
}

void AEnemyCharacter::StopRoarBlueFire()
{
	HasRoarBlueFire = false;
	Effect->Deactivate();
}

void AEnemyCharacter::Grab()
{
	
}

void AEnemyCharacter::Death()
{
	IsGettingHit = false;
	HasDied = true;
	MoveCompRef->DisableMovement();
	PlayPlayerEndGame();

}

void AEnemyCharacter::StopPunch()
{
	IsPunching = false;
}

void AEnemyCharacter::StopBigPunch()
{
	HasBigPunched = false;
}

void AEnemyCharacter::MoveToPlayer()
{
	if (PlayerRef)
	{
		FVector PlayerLocation = PlayerRef->GetActorLocation();
		PlayerLocation.Z = 0;
		
		EnemyAIController->MoveToLocation(PlayerLocation, StopDistance, true);
	}
	
}

void AEnemyCharacter::SeekPlayer()
{
	MoveToPlayer();
}

void AEnemyCharacter::StopSeekingPlayer()
{
//	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Death();
	}
	return DamageAmount;
}

void AEnemyCharacter::DealDamageTo(AActor* OtherActor)
{
	OtherActor->TakeDamage(4.0f, FDamageEvent(), GetInstigatorController(),this);
}


void AEnemyCharacter::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef)
	{
		PlayerDetected = true;
		if (!IsGettingHit)
		{
			SeekPlayer();
		}
	}
}



void AEnemyCharacter::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef)
	{
		PlayerDetected = false;
	}
}

void AEnemyCharacter::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef)
	{
		CanAttackPlayer = true;
	}
}

void AEnemyCharacter::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef)
	{
		CanAttackPlayer = false;
		if (!IsGettingHit)
		{
			SeekPlayer();
		}
	}
}

void AEnemyCharacter::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	CanDealDamage = true;
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef)
	{
	//	PlayerRef->IsGettingHit = false;
	}
	
	if (PlayerRef && CanAttackPlayer && PlayerDetected && OtherComp->GetName() == "CollisionBox2")
	{
		if ((CanDealDamage) && (IsPunching || HasBigPunched || HasJumpPunched))
		{
			MoveCompRef->DisableMovement();
			PlayerRef->MoveCompRef->DisableMovement();
			PlayerRef->IsGettingHit = true;
			PlayerRef->StopPunch();
			PlayerRef->StopBigPunch();
			DealDamageTo(PlayerRef);
			PlayerRef->LaunchCharacter(FVector(0, 200.0f, 0), false, false);
			
		}

		
	}
}

void AEnemyCharacter::OnDealDamageOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	CanDealDamage = false;
	PlayerRef = Cast<AFightingCharacter>(OtherActor);
	if (PlayerRef && OtherComp->GetName() == "CollisionBox2")
	{
		StopPunch();
		StopBigPunch();
		//PlayerRef->IsGettingHit = false;
		
	}
	
}


void AEnemyCharacter::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!PlayerDetected)
	{
	//	StopPunch();
		//SeekPlayer();
	}
	else if (PlayerDetected && CanAttackPlayer)
	{
		StopSeekingPlayer();
		int randNum = FMath::RandRange(1, 2);
		switch (randNum)
		{
		default:
			Punch();
		case 1:
			Punch();
		case 2:
			BigPunch();
			
		}
	}
	else if (PlayerDetected && !CanAttackPlayer)
	{
	//	StopPunch();
	//	SeekPlayer();
	}
	
}
