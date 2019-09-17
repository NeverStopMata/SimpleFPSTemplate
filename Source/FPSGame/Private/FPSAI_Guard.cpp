// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAI_Guard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
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
}

// Called every frame
void AFPSAI_Guard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}


void AFPSAI_Guard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
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
