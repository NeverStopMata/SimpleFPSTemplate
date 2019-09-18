// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAI_Guard.generated.h"

class UPawnSensingComponent;
UENUM(BlueprintType)
enum class EGuardState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAI_Guard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAI_Guard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category = "Component")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnSeePawn(APawn* InstigatorPawn);
	UFUNCTION()
	void OnHearNoise(APawn* InstigatorPawn, const FVector& Location, float Volume);

	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;
	EGuardState GuardState;

	void SetGuardState(EGuardState newState);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI")
	void OnStateChanged(EGuardState newState);

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;
	
	UPROPERTY(EditInstanceOnly,  Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* FirstTargetPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
	AActor* SecondTargetPoint;

	AActor* CurrentTargetPoint;

	void MoveTowardsNextTarget();
	void MoveTowardsCurrentTarget();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ResetOrientation();
};
