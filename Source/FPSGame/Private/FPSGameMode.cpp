// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}
void AFPSGameMode::CompleteMission(APawn* InstigatorPawn,bool bIsSuccessful)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);
		if (SpectatingViewpointClass)
		{
			TArray<AActor*> ReturnActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnActors);
			//change viewtarget if any valid actor is found.
			if (ReturnActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnActors[0];
				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}	
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("no viewpoint actor found in the scene"));
		}
	}
	OnMissionCompleted(InstigatorPawn, bIsSuccessful);
}
