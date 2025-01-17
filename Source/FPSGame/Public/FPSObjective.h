// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjective.generated.h"

class UStaticMeshComponent;
class USphereComponent;
UCLASS()
class FPSGAME_API AFPSObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjective();

protected:
	UPROPERTY(VisibleAnywhere,Category="Components")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere,Category="Components")
	USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* SelectEffect;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	void PlaySelectEffect();
public:	

};
