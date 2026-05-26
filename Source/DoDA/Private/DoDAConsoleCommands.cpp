#include "DoDAConsoleCommands.h"
#include "DoDASimOrchestrator.h"
#include "DoDAIdentity.h"
#include "DoDAPawn.h"
#include "DoDACase.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

void UDoDAConsoleCommands::RegisterCommands(UWorld* InWorld)
{
    World = InWorld;

    CmdDumpPawns = IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("doda.DumpPawns"),
        TEXT("Print all pawn vitals to log."),
        FConsoleCommandDelegate::CreateLambda([this]()
            {
                if (!World) return;
                UPawnSubsystem* PawnSys = World->GetSubsystem<UPawnSubsystem>();
                if (!PawnSys) return;
                for (const auto& Kvp : PawnSys->GetAllPawns())
                {
                    const FPawnRecord& P = Kvp.Value;
                    UE_LOG(LogTemp, Log,
                        TEXT("DoDA|Pawn %d -- Stamina:%.1f Stress:%.1f Sanity:%.1f Status:%d"),
                        P.PawnId.Value,
                        P.Vitals.StaminaCurrent,
                        P.Vitals.StressCurrent,
                        P.Vitals.SanityCurrent,
                        (int32)P.RoleState.CurrentStatus);
                }
            }),
        ECVF_Default
    );

    CmdDumpTasks = IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("doda.DumpTasks"),
        TEXT("Print all active tasks to log."),
        FConsoleCommandDelegate::CreateLambda([this]()
            {
                if (!World) return;
                UCaseSubsystem* CaseSys = World->GetSubsystem<UCaseSubsystem>();
                if (!CaseSys) return;
                TArray<FTask> Tasks = CaseSys->GetAllActiveTasks();
                for (const FTask& T : Tasks)
                {
                    UE_LOG(LogTemp, Log,
                        TEXT("DoDA|Task %d -- Case:%d Phase:%d Type:%d Status:%d Priority:%d"),
                        T.TaskId.Value,
                        T.CaseId.Value,
                        T.PhaseId.Value,
                        (int32)T.TaskType,
                        (int32)T.Status,
                        T.Priority);
                }
            }),
        ECVF_Default
    );

    CmdDumpSim = IConsoleManager::Get().RegisterConsoleCommand(
        TEXT("doda.DumpSim"),
        TEXT("Print sim state to log."),
        FConsoleCommandDelegate::CreateLambda([this]()
            {
                if (!World) return;
                UDoDASimOrchestrator* Orch = World->GetSubsystem<UDoDASimOrchestrator>();
                if (!Orch) return;
                UE_LOG(LogTemp, Log,
                    TEXT("DoDA|Sim -- Day:%d SimTime:%.1fs Paused:%d"),
                    Orch->GetSimDay(),
                    Orch->GetSimTimeSeconds(),
                    (int32)Orch->IsSimPaused());
            }),
        ECVF_Default
    );
}