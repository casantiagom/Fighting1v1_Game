// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightingCharacter.h"
#include "MutantAnimNotify.h"
#include "AkAudioEvent.h"
#include "AkComponent.h"
#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"



AFightingCharacter::AFightingCharacter()
{
	// 
	Health = DefaultHealth;
	HeadSocket = TEXT("HeadSocket");
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// Collision boxes
	HitBox2 = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBox2"));
	HitBox2->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageBox2 = CreateDefaultSubobject<UBoxComponent>(FName("Damage left Collision"));
	DamageBox2->SetupAttachment(GetMesh(), TEXT("LeftHandSocket"));
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Right Collision"));
	DamageBox->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	
	EnemyCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));
	EnemyCollision->SetupAttachment(RootComponent);
	EnemyAttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));
	EnemyAttackCollision->SetupAttachment(RootComponent);
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	//Character Movement setup
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindSong(TEXT("/Game/WwiseAudio/LevelSong.LevelSong"));
	Song = FindSong.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindSongStop(TEXT("/Game/WwiseAudio/SongStop.SongStop"));
	SongStop = FindSongStop.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindEndSong(TEXT("/Game/WwiseAudio/EndSong.EndSong"));
	EndSong = FindEndSong.Object;

	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindJumpAudioEvent(TEXT("/Game/WwiseAudio/Jump.Jump"));
	JumpAudioEvent = FindJumpAudioEvent.Object;
	
	static ConstructorHelpers::FObjectFinder<UAkAudioEvent> FindBigPunchEvent(TEXT("/Game/WwiseAudio/M_BigPunch.M_BigPunch"));
	M_BigPunchAudioEvent = FindBigPunchEvent.Object;
}

void AFightingCharacter::BeginPlay()
{
	Super::BeginPlay();

	FOnAkPostEventCallback nullCallback;
	
	UAkComponent* AkComponent = NewObject<UAkComponent>(this);
	
	//UAkAudioEvent* M_Attack = UAkAudioDevice
	//M_Attack->PostOnComponent()

	

	if (Song != nullptr)
	{
		Song->PostOnActor(this, nullCallback,int32(0), true);
		
	}
	
	MoveCompRef = GetCharacterMovement();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Niagara Blue Fire Effect
	if (NiagaraFX->IsValid())
	{
		Effect = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraFX, this->GetMesh(), HeadSocket, FVector(0.0f, 0.0f, 20.0f), FRotator(-200.0f), EAttachLocation::Type::SnapToTarget, false, false);
		Effect->Deactivate();
	}

	//Binding collision with overlaps
	DamageBox->OnComponentBeginOverlap.AddDynamic(this,
		&AFightingCharacter::OnDealDamageOverlapBegin);

	DamageBox->OnComponentEndOverlap.AddDynamic(this,
		&AFightingCharacter::OnDealDamageOverlapEnd);

	DamageBox2->OnComponentBeginOverlap.AddDynamic(this,
		&AFightingCharacter::OnDealDamageOverlapBegin);

	DamageBox2->OnComponentEndOverlap.AddDynamic(this,
		&AFightingCharacter::OnDealDamageOverlapEnd);
	
}

	


void AFightingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFightingCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFightingCharacter::Look);
		//Punch
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Triggered, this, &AFightingCharacter::Punch);
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Completed, this, &AFightingCharacter::StopPunch);
		//BigPunch
		EnhancedInputComponent->BindAction(BigPunchAction, ETriggerEvent::Triggered, this, &AFightingCharacter::BigPunch);
		EnhancedInputComponent->BindAction(BigPunchAction, ETriggerEvent::Completed, this, &AFightingCharacter::StopBigPunch);
		//Roar Blue fire
		EnhancedInputComponent->BindAction(RoarBlueFireAction, ETriggerEvent::Triggered, this, &AFightingCharacter::RoarBlueFire);
		EnhancedInputComponent->BindAction(RoarBlueFireAction, ETriggerEvent::Completed, this, &AFightingCharacter::StopRoarBlueFire);
	}

}

void AFightingCharacter::Move(const FInputActionValue& Value)
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

void AFightingCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFightingCharacter::Punch()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(NULL);
	StopPunch();
	IsPunching = true;
}

void AFightingCharacter::BigPunch()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(NULL);
	StopBigPunch();
	HasBigPunched = true;
	
}

void AFightingCharacter::PlayEndGame()
{
	FOnAkPostEventCallback nullCallback;	
	SongStop->PostOnActor(this, nullCallback,int32(0), true);
	EndSong->PostOnActor(this, nullCallback,int32(0), true);
}

void AFightingCharacter::RoarBlueFire()
{
	HasRoarBlueFire = true;
	Effect->Activate();
}

void AFightingCharacter::StopRoarBlueFire()
{
	HasRoarBlueFire = false;
	Effect->Deactivate();
	FOnAkPostEventCallback nullCallback;	
	
	if (M_Fire_Loop)
	{
		M_Fire_Loop->ExecuteAction(AkActionOnEventType::Stop,this,0,0,EAkCurveInterpolation::LastFadeCurve);
	}

	if (M_Fire_Loop_End)
	{
		M_Fire_Loop_End->PostOnActor(this, nullCallback,int32(0), true);
	}

	
}

void AFightingCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	FOnAkPostEventCallback nullCallback;
	Arrive->PostOnActor(this, nullCallback,int32(0), true);
}

void AFightingCharacter::Jump()
{
	Super::Jump();
	FOnAkPostEventCallback nullCallback;
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	if (!MoveCompRef->IsFalling())
	{
		JumpAudioEvent->PostOnActor(this, nullCallback,int32(0), true);

	}
	
}

void AFightingCharacter::Death()
{
	HasDied = true;
	PlayEndGame();
}

float AFightingCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	//GEngine->AddOnScreenDebugMessage(-1,4.5f, FColor::Yellow, FString::SanitizeFloat(Health));

	if (Health <= 0)
	{
		HasDied = true;
		MoveCompRef->DisableMovement();
	}
	return DamageAmount;
}


void AFightingCharacter::DealDamageTo(AActor* OtherActor, float DamageAmount)
{
	OtherActor->TakeDamage(5.0f, FDamageEvent(), GetInstigatorController(),this);
}

void AFightingCharacter::StopPunch()
{
	IsPunching = false;
}

void AFightingCharacter::StopBigPunch()
{
	HasBigPunched = false;
}


void AFightingCharacter::EnableWalk()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	MoveComp->SetMovementMode(MOVE_Walking);
}


void AFightingCharacter::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	CanDealDamage = true;
	EnemyRef = Cast<AEnemyCharacter>(OtherActor);

	FOnAkPostEventCallback nullCallback;

	
	
	if (EnemyRef && CanDealDamage && OtherComp->GetName() == "CollisionBox2")
	{
		if (HasBigPunched)
		{
			EnemyRef->StopPunch();
			EnemyRef->StopBigPunch();
			EnemyRef->IsGettingHit = true;
			EnemyRef->LaunchCharacter(FVector(0, 200.0f, 0), false, false);
			DealDamageTo(EnemyRef, 14.0f);
		}
	}
	
	if (EnemyRef && CanDealDamage && OtherComp->GetName() == "CollisionBox2")
	{
		if (IsPunching || HasRoarBlueFire || HasBigPunched)
		{
			EnemyRef->StopPunch();
			EnemyRef->StopBigPunch();
			EnemyRef->IsGettingHit = true;
			EnemyRef->LaunchCharacter(FVector(0, 200.0f, 0), false, false);

			if (IsPunching)
			{
				DealDamageTo(EnemyRef, 5.0f);
			}
			if (HasRoarBlueFire)
			{
				DealDamageTo(EnemyRef, 15.0f);
			}

			if (HasBigPunched)
			{
				DealDamageTo(EnemyRef, 20.0f);
			}
		}
	}
}

void AFightingCharacter::OnDealDamageOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CanDealDamage = false;
	EnemyRef = Cast<AEnemyCharacter>(OtherActor);
	if (EnemyRef && OtherComp->GetName() == "CollisionBox2")
	{
		//EnemyRef->IsGettingHit = false;
		//EnemyRef->EnableWalk();
		//EnemyRef->EnableWalk();
	}

	
}

