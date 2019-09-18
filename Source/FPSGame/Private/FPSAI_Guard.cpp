// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAI_Guard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
// Sets default values
AFPSAI_Guard::AFPSAI_Guard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAI_Guard::OnSeePawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAI_Guard::OnHearNoise);
	GuardState = EGuardState::Idle;
}

// Called when the game starts or when spawned
void AFPSAI_Guard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();	
	if (bPatrol)
	{
		MoveTowardsNextTarget();
	}
}

// Called every frame
void AFPSAI_Guard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTargetPoint)
	{
		FVector delta = GetActorLocation() - CurrentTargetPoint->GetActorLocation();
		float distance = delta.Size();
		if (distance < 200)
		{
			MoveTowardsNextTarget();
		}
	}
}

void AFPSAI_Guard::OnSeePawn(APawn* InstigatorPawn)
{
	if (InstigatorPawn == nullptr)
	{
		return;
	}

	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(InstigatorPawn);
	if (MyCharacter == nullptr)
	{
		return;
	}
	SetGuardState(EGuardState::Alerted);
	DrawDebugSphere(GetWorld(), InstigatorPawn->GetActorLocation(), 32, 12, FColor::Red, false, 10.0f);
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(InstigatorPawn, false);
	}
	AController* controller = GetController();
	if (controller)
	{
		controller->StopMovement();
	}
		
}
void AFPSAI_Guard::OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume)
{
	if (GuardState == EGuardState::Alerted)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), Location, 32, 12, FColor::Green, false, 10.0f);
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAI_Guard::ResetOrientation, 3.0f);
	SetGuardState(EGuardState::Suspicious);
	AController* controller = GetController();
	if (controller)
	{
		controller->StopMovement();
	}
}


void AFPSAI_Guard::ResetOrientation()
{
	if (GuardState == EGuardState::Alerted)
	{
		return;
	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EGuardState::Idle);
	if (bPatrol)
	{
		MoveTowardsCurrentTarget();
	}
}

void AFPSAI_Guard::SetGuardState(EGuardState newState)
{
	if (newState == GuardState)
	{
		return;
	}
	GuardState = newState;
	OnStateChanged(newState);
}

void AFPSAI_Guard::MoveTowardsNextTarget()
{
	if (CurrentTargetPoint == SecondTargetPoint || CurrentTargetPoint == nullptr)
	{
		CurrentTargetPoint = FirstTargetPoint;
	}
	else if (CurrentTargetPoint == FirstTargetPoint)
	{
		CurrentTargetPoint = SecondTargetPoint;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("current target point is invalid"));
	}
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
}

void AFPSAI_Guard::MoveTowardsCurrentTarget()
{
	if (CurrentTargetPoint)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
	}
}