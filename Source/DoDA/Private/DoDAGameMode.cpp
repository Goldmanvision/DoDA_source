#include "DoDAGameMode.h"
#include "DoDAIdentity.h"
#include "DoDADebugHUD.h"
#include "DoDAPawn.h"
#include "DoDACase.h"
#include "DoDAMap.h"
#include "Engine/World.h"

ADoDAGameMode::ADoDAGameMode()
{
    HUDClass = ADoDADebugHUD::StaticClass();
}

void ADoDAGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    // Defer seeding one frame to ensure subsystems are ready
    World->GetTimerManager().SetTimerForNextTick([World]()
        {
            UIdentitySubsystem* IdSys = World->GetSubsystem<UIdentitySubsystem>();
            if (IdSys)
            {
                IdSys->SeedTestIdentities(5);
                UE_LOG(LogTemp, Log, TEXT("DoDA|GameMode -- seeded 5 identities"));
            }

            UPawnSubsystem* PawnSys = World->GetSubsystem<UPawnSubsystem>();
            if (PawnSys)
            {
                PawnSys->SeedTestPawns(5);
                UE_LOG(LogTemp, Log, TEXT("DoDA|GameMode -- seeded %d pawns"), PawnSys->Count());
            }

            UMapSubsystem* MapSys = World->GetSubsystem<UMapSubsystem>();
            if (MapSys)
            {
                MapSys->GenerateTown(42);
                UE_LOG(LogTemp, Log, TEXT("DoDA|GameMode -- town generated"));
            }

            UCaseSubsystem* CaseSys = World->GetSubsystem<UCaseSubsystem>();
            if (CaseSys)
            {
                FCaseId CId = CaseSys->CreateCase(TEXT("Case 001 - Brightstar Daycare"), 2, 42);
                FCasePhaseId PhaseId = CaseSys->AddPhase(CId, EPhaseType::Intake, 0.f);

                CaseSys->AddTask(CId, PhaseId, ETaskType::Surveillance, 1);
                CaseSys->AddTask(CId, PhaseId, ETaskType::Interview, 2);
                CaseSys->AddTask(CId, PhaseId, ETaskType::Paperwork, 3);
                CaseSys->AddTask(CId, PhaseId, ETaskType::ForensicSweep, 2);
                CaseSys->AddTask(CId, PhaseId, ETaskType::Research, 1);

                UE_LOG(LogTemp, Log, TEXT("DoDA|GameMode -- case seeded id=%d, 5 tasks added"), CId.Value);
            }
        });
}