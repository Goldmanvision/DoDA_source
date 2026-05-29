// Source/DoDA/Private/DoDAScheduler.cpp
// Batch 05 -- ASCII-only.

#include "DoDAScheduler.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UDoDASchedulerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UCaseSubsystem* CaseSys = GetWorld()->GetSubsystem<UCaseSubsystem>();
    if (CaseSys)
    {
        CaseSys->OnTaskComplete.AddUObject(this, &UDoDASchedulerSubsystem::OnTaskCompleted);
    }
}

bool UDoDASchedulerSubsystem::IsAgentAssignable(const FAgentView& Agent) const
{
    if (Agent.Status != EPawnStatus::Active) return false;

    switch (Agent.WorkState)
    {
    case EWorkState::Available:
        return true;
    case EWorkState::Busy:
    case EWorkState::InTransit:
    case EWorkState::OnSite:
    case EWorkState::Injured:
    case EWorkState::Recovering:
    case EWorkState::Detached:
    default:
        return false;
    }
}

void UDoDASchedulerSubsystem::RunScheduler()
{
    Assignments.Empty();

    UCaseSubsystem* CaseSys = GetWorld()->GetSubsystem<UCaseSubsystem>();
    if (!CaseSys) return;

    UPawnSubsystem* PawnSys = GetWorld()->GetSubsystem<UPawnSubsystem>();
    if (!PawnSys) return;

    TArray<FAgentView> Agents = BuildAgentViews();
    TArray<bool> AgentUsed;
    AgentUsed.Init(false, Agents.Num());

    TArray<FTask> OpenTasks = CaseSys->GetPendingTasks();

    for (const FTask& Task : OpenTasks)
    {
        float BestCost = MAX_FLT;
        int32 BestIdx = INDEX_NONE;

        for (int32 i = 0; i < Agents.Num(); i++)
        {
            if (AgentUsed[i]) continue;
            if (!IsAgentAssignable(Agents[i])) continue;

            float Cost = ComputeCost(Agents[i], Task);
            if (Cost < BestCost)
            {
                BestCost = Cost;
                BestIdx = i;
            }
        }

        if (BestIdx != INDEX_NONE)
        {
            AgentUsed[BestIdx] = true;

            FSchedulerAssignment A;
            A.TaskId = Task.TaskId;
            A.PawnId = Agents[BestIdx].PawnId;
            A.Cost = BestCost;
            Assignments.Add(A);

            CaseSys->SetTaskStatus(Task.TaskId, ETaskStatus::InProgress);

            // Mark pawn as busy in pawn subsystem
            FPawnRecord* Pawn = PawnSys->GetPawnMutable(Agents[BestIdx].PawnId);
            if (Pawn)
            {
                Pawn->RoleState.WorkState = EWorkState::Busy;
            }

            UE_LOG(LogTemp, Log, TEXT("DoDA|Scheduler -- Task %d -> Pawn %d (cost %.1f)"),
                Task.TaskId.Value, Agents[BestIdx].PawnId.Value, BestCost);
        }
    }
}

TArray<FAgentView> UDoDASchedulerSubsystem::BuildAgentViews() const
{
    TArray<FAgentView> Views;

    UPawnSubsystem* PawnSys = GetWorld()->GetSubsystem<UPawnSubsystem>();
    if (!PawnSys) return Views;

    for (const TPair<FPawnId, FPawnRecord>& Pair : PawnSys->GetAllPawns())
    {
        const FPawnRecord& Pawn = Pair.Value;

        FAgentView V;
        V.PawnId = Pawn.PawnId;
        V.RoleClass = Pawn.RoleState.RoleClass;
        V.Stress = Pawn.Vitals.StressCurrent;
        V.Stamina = Pawn.Vitals.StaminaCurrent;
        V.Status = Pawn.RoleState.CurrentStatus;
        V.WorkState = Pawn.RoleState.WorkState;
        V.Investigation = Pawn.Skills.Investigation;
        V.Medicine = Pawn.Skills.Medicine;
        V.OccultKnowledge = Pawn.Skills.OccultKnowledge;
        V.Surveillance = Pawn.Skills.Surveillance;
        V.Interviewing = Pawn.Skills.Interviewing;

        Views.Add(V);
    }

    return Views;
}

float UDoDASchedulerSubsystem::ComputeCost(const FAgentView& Agent, const FTask& Task) const
{
    float Cost = 100.f;

    switch (Task.TaskType)
    {
    case ETaskType::Surveillance:
        Cost -= Agent.Surveillance * 0.5f;
        break;
    case ETaskType::Interview:
        Cost -= Agent.Interviewing * 0.5f;
        break;
    case ETaskType::ForensicSweep:
        Cost -= Agent.Investigation * 0.4f;
        break;
    case ETaskType::Research:
        Cost -= Agent.OccultKnowledge * 0.5f;
        break;
    case ETaskType::Arrest:
        Cost -= Agent.Investigation * 0.3f;
        break;
    case ETaskType::Containment:
        Cost -= Agent.OccultKnowledge * 0.4f;
        break;
    case ETaskType::Paperwork:
        Cost -= Agent.Investigation * 0.2f;
        break;
    case ETaskType::Negotiation:
        Cost -= Agent.Interviewing * 0.4f;
        break;
    case ETaskType::Infiltration:
        Cost -= Agent.Surveillance * 0.4f;
        break;
    default:
        break;
    }

    Cost += Agent.Stress * 0.3f;
    Cost -= Agent.Stamina * 0.1f;

    return FMath::Max(0.f, Cost);
}

void UDoDASchedulerSubsystem::OnTaskCompleted(FTaskId TaskId, FCaseId CaseId)
{
    // Find the pawn assigned to this task and free their work state
    UPawnSubsystem* PawnSys = GetWorld()->GetSubsystem<UPawnSubsystem>();

    for (const FSchedulerAssignment& A : Assignments)
    {
        if (A.TaskId == TaskId && PawnSys)
        {
            FPawnRecord* Pawn = PawnSys->GetPawnMutable(A.PawnId);
            if (Pawn && Pawn->RoleState.CurrentStatus == EPawnStatus::Active)
            {
                Pawn->RoleState.WorkState = EWorkState::Available;
            }
        }
    }

    Assignments.RemoveAll([&TaskId](const FSchedulerAssignment& A)
        {
            return A.TaskId == TaskId;
        });

    UE_LOG(LogTemp, Log, TEXT("DoDA|Scheduler -- Task %d freed (Case %d)"),
        TaskId.Value, CaseId.Value);
}