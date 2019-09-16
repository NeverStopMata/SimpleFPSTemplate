// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjective.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
// Sets default values
AFPSObjective::AFPSObjective()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = MeshComp;
	SphereComp->SetupAttachment(MeshComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AFPSObjective::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSObjective::PlaySelectEffect()
{
	UGameplayStatics::SpawnEmitterAtLocation(this->GetWorld(), SelectEffect, this->GetActorLocation());
}


void AFPSObjective::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	AFPSCharacter* myCharacter = Cast<AFPSCharacter>(OtherActor);
	if (myCharacter)
	{
		myCharacter->bIsCarryingObjective = true;
		PlaySelectEffect();
		Destroy();
	}
}

