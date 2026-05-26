#include "DoDASimOrchestrator.h"
#include "DoDAPawn.h"
#include "DoDACult.h"
#include "DoDAScheduler.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UDoDASimOrchestrator::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    SimTimeSeconds          = 0.f;
    TimeSinceLastScheduler  = 0.f;
    TimeSinceLastCultStep   = 0.f;
    bPaused                 = false;
    TimeScale               = 1.f;

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
    SimTimeSeconds    += DeltaSimTime;

    TickVitals(DeltaSimTime);
    TickCult(DeltaSimTime);
    TickScheduler(DeltaSimTime);
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

    UE_LOG(LogTemp, Log, TEXT("DoDA|Orchestrator -- cult/paranormal step at sim-day %d"), GetSimDay());
}

void UDoDASimOrchestrator::TickScheduler(float DeltaSimTime)
{
    TimeSinceLastScheduler += DeltaSimTime;
    if (TimeSinceLastScheduler < SchedulerInterval) return;

    TimeSinceLastScheduler = 0.f;

    UDoDASchedulerSubsystem* Sched = GetWorld()->GetSubsystem<UDoDASchedulerSubsystem>();
    if (Sched)
    {
        Sched->RunScheduler();
    }

    UE_LOG(LogTemp, Log, TEXT("DoDA|Orchestrator -- scheduler pass at sim-day %d"), GetSimDay());
}