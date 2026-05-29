// Source/DoDA/Private/DoDASimOrchestrator.cpp
// Batch 06 -- ASCII-only.

#include "DoDASimOrchestrator.h"
#include "DoDAPawn.h"
#include "DoDACult.h"
#include "DoDAScheduler.h"
#include "DoDACase.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UDoDASimOrchestrator::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    SimTimeSeconds = 0.f;
    TimeSinceLastScheduler = 0.f;
    TimeSinceLastCultStep = 0.f;
    TimeSinceLastCUIPulse = 0.f;
    bPaused = false;
    TimeScale = 1.f;
    SchedulerInterval = FMath::RandRange(SchedulerIntervalMin, SchedulerIntervalMax);

    UE_LOG(LogTemp, Log, TEXT("DoDA|Orchestrator -- initialized"));
}

void UDoDASimOrchestrator::Deinitialize()
{
    Super::Deinitialize();
}

TStatId UDoDASimOrchestrator::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UDoDASimOrchestrator, STATGROUP_Tickables);
}

void UDoDASimOrchestrator::Tick(float DeltaTime)
{
    if (bPaused) return;

    float DeltaSimTime = DeltaTime * TimeScale;
    SimTimeSeconds += DeltaSimTime;

    TickVitals(DeltaSimTime);
    TickCult(DeltaSimTime);
    TickScheduler(DeltaSimTime);
    TickTasks(DeltaSimTime);
    TickCUI(DeltaSimTime);
}

void UDoDASimOrchestrator::TickVitals(float DeltaSimTime)
{
    UPawnSubsystem* PawnSys = GetWorld()->GetSubsystem<UPawnSubsystem>();
    if (PawnSys)
    {
        PawnSys->UpdateVitals(DeltaSimTime);
    }
}

void UDoDASimOrchestrator::TickCult(float DeltaSimTime)
{
    TimeSinceLastCultStep += DeltaSimTime;
    if (TimeSinceLastCultStep < CultStepInterval) return;

    TimeSinceLastCultStep = 0.f;

    UCultSubsystem* CultSys = GetWorld()->GetSubsystem<UCultSubsystem>();
    if (CultSys)
    {
        CultSys->StepCult(CultStepInterval);
        CultSys->StepParanormal(CultStepInterval);
    }

    DispatchAlert(TEXT("Cult"), FString::Printf(TEXT("Cult/paranormal step at sim-day %d"), GetSimDay()), EDoDAAlertSeverity::Info);
}

void UDoDASimOrchestrator::TickScheduler(float DeltaSimTime)
{
    TimeSinceLastScheduler += DeltaSimTime;
    if (TimeSinceLastScheduler < SchedulerInterval) return;

    TimeSinceLastScheduler = 0.f;
    SchedulerInterval = FMath::RandRange(SchedulerIntervalMin, SchedulerIntervalMax);

    UDoDASchedulerSubsystem* Sched = GetWorld()->GetSubsystem<UDoDASchedulerSubsystem>();
    if (Sched)
    {
        Sched->RunScheduler();
    }

    DispatchAlert(TEXT("Scheduler"), FString::Printf(TEXT("Scheduler pass at sim-day %d, next in %.1fs"), GetSimDay(), SchedulerInterval), EDoDAAlertSeverity::Info);
}

void UDoDASimOrchestrator::TickTasks(float DeltaSimTime)
{
    UCaseSubsystem* Cases = GetWorld()->GetSubsystem<UCaseSubsystem>();
    if (!Cases) return;

    TArray<FTask> ActiveTasks = Cases->GetAllActiveTasks();
    for (const FTask& Task : ActiveTasks)
    {
        Cases->TickTaskProgress(Task.TaskId, DeltaSimTime);
    }
}

void UDoDASimOrchestrator::TickCUI(float DeltaSimTime)
{
    TimeSinceLastCUIPulse += DeltaSimTime;
    if (TimeSinceLastCUIPulse < CUIPulseInterval) return;

    TimeSinceLastCUIPulse = 0.f;

    UCaseSubsystem* Cases = GetWorld()->GetSubsystem<UCaseSubsystem>();
    UPawnSubsystem* Pawns = GetWorld()->GetSubsystem<UPawnSubsystem>();
    UCultSubsystem* Cult = GetWorld()->GetSubsystem<UCultSubsystem>();

    int32 OpenCases = Cases ? Cases->GetAllCases().Num() : 0;
    int32 PawnCount = Pawns ? Pawns->Count() : 0;
    int32 CultCells = Cult ? Cult->GetAllCells().Num() : 0;

    DispatchAlert(
        TEXT("CUI"),
        FString::Printf(TEXT("Pulse at day %d | Cases:%d Pawns:%d CultCells:%d"), GetSimDay(), OpenCases, PawnCount, CultCells),
        EDoDAAlertSeverity::Info
    );
}

void UDoDASimOrchestrator::DispatchAlert(const FString& Source, const FString& Message, EDoDAAlertSeverity Severity)
{
    FDoDAAlert Alert;
    Alert.Source = Source;
    Alert.Message = Message;
    Alert.Severity = Severity;
    Alert.SimTimeSeconds = SimTimeSeconds;

    OnAlert.Broadcast(Alert);

    UE_LOG(LogTemp, Log, TEXT("DoDA|Alert -- [%s] %s"), *Source, *Message);
}