// Source/DoDA/Private/DoDACase.cpp
// Batch 05 -- ASCII-only.

#include "DoDACase.h"

void UCaseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Cases.Empty();
    Phases.Empty();
    Tasks.Empty();
    Assignments.Empty();
    Evidences.Empty();
    Warrants.Empty();
    Tracks.Empty();
    NextCaseId = NextPhaseId = NextTaskId = 1;
    NextAssignId = NextEvidenceId = NextWarrantId = 1;
}

void UCaseSubsystem::Deinitialize()
{
    Cases.Empty();
    Phases.Empty();
    Tasks.Empty();
    Assignments.Empty();
    Evidences.Empty();
    Warrants.Empty();
    Tracks.Empty();
    Super::Deinitialize();
}

FCaseId UCaseSubsystem::AllocateCaseId()
{
    FCaseId Id;
    Id.Value = NextCaseId++;
    return Id;
}

FCasePhaseId UCaseSubsystem::AllocatePhaseId()
{
    FCasePhaseId Id;
    Id.Value = NextPhaseId++;
    return Id;
}

FTaskId UCaseSubsystem::AllocateTaskId()
{
    FTaskId Id;
    Id.Value = NextTaskId++;
    return Id;
}

FAssignmentId UCaseSubsystem::AllocateAssignmentId()
{
    FAssignmentId Id;
    Id.Value = NextAssignId++;
    return Id;
}

FEvidenceId UCaseSubsystem::AllocateEvidenceId()
{
    FEvidenceId Id;
    Id.Value = NextEvidenceId++;
    return Id;
}

FWarrantId UCaseSubsystem::AllocateWarrantId()
{
    FWarrantId Id;
    Id.Value = NextWarrantId++;
    return Id;
}

FCaseId UCaseSubsystem::CreateCase(const FString& Title, int32 Difficulty, int32 Seed)
{
    FCase C;
    C.CaseId = AllocateCaseId();
    C.Title = Title;
    C.Status = ECaseStatus::Open;
    C.Difficulty = Difficulty;
    C.Seed = Seed;
    C.Progress = 0;
    C.Risk = 0;
    Cases.Add(C.CaseId, C);

    FCaseTrack Track;
    Track.CaseId = C.CaseId;
    Tracks.Add(C.CaseId, Track);

    return C.CaseId;
}

FCase* UCaseSubsystem::GetCaseMutable(FCaseId Id)
{
    return Cases.Find(Id);
}

const FCase* UCaseSubsystem::GetCase(FCaseId Id) const
{
    return Cases.Find(Id);
}

FCasePhaseId UCaseSubsystem::AddPhase(FCaseId CaseId, EPhaseType PhaseType, float StartTime)
{
    FCasePhase P;
    P.PhaseId = AllocatePhaseId();
    P.CaseId = CaseId;
    P.PhaseType = PhaseType;
    P.StartTime = StartTime;
    P.EndTime = 0.f;
    Phases.Add(P.PhaseId, P);

    FCase* Case = GetCaseMutable(CaseId);
    if (Case && !Case->CurrentPhaseId.IsValid())
    {
        Case->CurrentPhaseId = P.PhaseId;
    }

    return P.PhaseId;
}

TArray<FCasePhase> UCaseSubsystem::GetPhasesForCase(FCaseId CaseId) const
{
    TArray<FCasePhase> Result;
    for (const auto& Pair : Phases)
    {
        if (Pair.Value.CaseId == CaseId)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

void UCaseSubsystem::SetCurrentPhase(FCaseId CaseId, FCasePhaseId PhaseId)
{
    FCase* Case = GetCaseMutable(CaseId);
    if (!Case) return;
    Case->CurrentPhaseId = PhaseId;
}

void UCaseSubsystem::AddCaseProgress(FCaseId CaseId, int32 Amount)
{
    FCase* Case = GetCaseMutable(CaseId);
    if (!Case) return;

    Case->Progress = FMath::Clamp(Case->Progress + Amount, 0, 100);

    if (Case->Progress >= 100 && Case->Status == ECaseStatus::Open)
    {
        Case->Status = ECaseStatus::Active;
    }
}

void UCaseSubsystem::AddCaseRisk(FCaseId CaseId, int32 Amount)
{
    FCase* Case = GetCaseMutable(CaseId);
    if (!Case) return;

    Case->Risk = FMath::Clamp(Case->Risk + Amount, 0, 100);
}

void UCaseSubsystem::TickCaseProgress(FCaseId CaseId, float DeltaSimTime)
{
    FCase* Case = GetCaseMutable(CaseId);
    if (!Case) return;

    if (Case->Status == ECaseStatus::Closed || Case->Status == ECaseStatus::Shelved)
    {
        return;
    }

    int32 ProgressGain = FMath::RoundToInt(DeltaSimTime * 0.02f);
    int32 RiskGain = FMath::RoundToInt(DeltaSimTime * 0.01f);

    AddCaseProgress(CaseId, ProgressGain);
    AddCaseRisk(CaseId, RiskGain);

    if (Case->Risk >= 80 && Case->Status == ECaseStatus::Open)
    {
        Case->Status = ECaseStatus::Compromised;
    }
}

FTaskId UCaseSubsystem::AddTask(FCaseId CaseId, FCasePhaseId PhaseId, ETaskType Type, int32 Priority)
{
    FTask T;
    T.TaskId = AllocateTaskId();
    T.CaseId = CaseId;
    T.PhaseId = PhaseId;
    T.TaskType = Type;
    T.Status = ETaskStatus::Pending;
    T.Priority = Priority;
    T.WorkAccumulated = 0.f;
    T.WorkRequired = 100.f;
    Tasks.Add(T.TaskId, T);
    return T.TaskId;
}

TArray<FTask> UCaseSubsystem::GetActiveTasksForCase(FCaseId CaseId) const
{
    TArray<FTask> Result;
    for (const auto& Pair : Tasks)
    {
        if (Pair.Value.CaseId == CaseId &&
            Pair.Value.Status != ETaskStatus::Complete &&
            Pair.Value.Status != ETaskStatus::Failed)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FTask> UCaseSubsystem::GetAllActiveTasks() const
{
    TArray<FTask> Result;
    for (const auto& Pair : Tasks)
    {
        if (Pair.Value.Status != ETaskStatus::Complete &&
            Pair.Value.Status != ETaskStatus::Failed)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FTask* UCaseSubsystem::GetTaskMutable(FTaskId Id)
{
    return Tasks.Find(Id);
}

FAssignmentId UCaseSubsystem::AddAssignment(FTaskId TaskId, FPawnId AgentId, float Cost, float StartTime, float EndTime)
{
    FAssignment A;
    A.AssignmentId = AllocateAssignmentId();
    A.TaskId = TaskId;
    A.AgentId = AgentId;
    A.Cost = Cost;
    A.StartTime = StartTime;
    A.EndTime = EndTime;
    A.Outcome = EAssignmentOutcome::Pending;
    Assignments.Add(A.AssignmentId, A);
    return A.AssignmentId;
}

TArray<FAssignment> UCaseSubsystem::GetAssignmentsForTask(FTaskId TaskId) const
{
    TArray<FAssignment> Result;
    for (const auto& Pair : Assignments)
    {
        if (Pair.Value.TaskId == TaskId)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FEvidenceId UCaseSubsystem::AddEvidence(FCaseId CaseId, const FString& Desc, EEvidenceType Type, FRoomId LocationId)
{
    FEvidence E;
    E.EvidenceId = AllocateEvidenceId();
    E.CaseId = CaseId;
    E.Description = Desc;
    E.EvidenceType = Type;
    E.EvidenceStatus = EEvidenceStatus::Uncollected;
    E.LocationId = LocationId;
    Evidences.Add(E.EvidenceId, E);
    return E.EvidenceId;
}

TArray<FEvidence> UCaseSubsystem::GetEvidenceForCase(FCaseId CaseId) const
{
    TArray<FEvidence> Result;
    for (const auto& Pair : Evidences)
    {
        if (Pair.Value.CaseId == CaseId)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FEvidence* UCaseSubsystem::GetEvidenceMutable(FEvidenceId Id)
{
    return Evidences.Find(Id);
}

FWarrantId UCaseSubsystem::AddWarrant(FCaseId CaseId, EWarrantType Type, const FString& Target, float IssuedTime, float ExpiryTime)
{
    FWarrant W;
    W.WarrantId = AllocateWarrantId();
    W.CaseId = CaseId;
    W.WarrantType = Type;
    W.WarrantStatus = EWarrantStatus::Pending;
    W.Target = Target;
    W.IssuedTime = IssuedTime;
    W.ExpiryTime = ExpiryTime;
    Warrants.Add(W.WarrantId, W);
    return W.WarrantId;
}

TArray<FWarrant> UCaseSubsystem::GetWarrantsForCase(FCaseId CaseId) const
{
    TArray<FWarrant> Result;
    for (const auto& Pair : Warrants)
    {
        if (Pair.Value.CaseId == CaseId)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FWarrant* UCaseSubsystem::GetWarrantMutable(FWarrantId Id)
{
    return Warrants.Find(Id);
}

FCaseTrack* UCaseSubsystem::GetTrackMutable(FCaseId CaseId)
{
    return Tracks.Find(CaseId);
}

const FCaseTrack* UCaseSubsystem::GetTrack(FCaseId CaseId) const
{
    return Tracks.Find(CaseId);
}

void UCaseSubsystem::TickTaskProgress(FTaskId TaskId, float DeltaSimTime)
{
    FTask* Task = GetTaskMutable(TaskId);
    if (!Task) return;
    if (Task->Status != ETaskStatus::InProgress) return;

    Task->WorkAccumulated += DeltaSimTime;

    FCase* Case = GetCaseMutable(Task->CaseId);
    if (Case)
    {
        AddCaseProgress(Task->CaseId, FMath::RoundToInt(DeltaSimTime * 0.01f));
        AddCaseRisk(Task->CaseId, FMath::RoundToInt(DeltaSimTime * 0.005f));
    }

    if (Task->WorkAccumulated >= Task->WorkRequired)
    {
        Task->Status = ETaskStatus::Complete;

        if (Case)
        {
            AddCaseProgress(Task->CaseId, 5);
        }

        UE_LOG(LogTemp, Log, TEXT("DoDA|Case -- Task %d Complete (Case %d)"),
            TaskId.Value, Task->CaseId.Value);
        OnTaskComplete.Broadcast(TaskId, Task->CaseId);
    }
}

void UCaseSubsystem::SetTaskStatus(FTaskId TaskId, ETaskStatus NewStatus)
{
    FTask* Task = GetTaskMutable(TaskId);
    if (!Task) return;
    Task->Status = NewStatus;
    UE_LOG(LogTemp, Log, TEXT("DoDA|Case -- Task %d status -> %d"),
        TaskId.Value, (uint8)NewStatus);
}

TArray<FTask> UCaseSubsystem::GetPendingTasks() const
{
    TArray<FTask> Result;
    for (const auto& Pair : Tasks)
    {
        if (Pair.Value.Status == ETaskStatus::Pending)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}