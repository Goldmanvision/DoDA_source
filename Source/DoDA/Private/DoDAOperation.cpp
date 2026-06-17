// Source/DoDA/Private/DoDAOperation.cpp

#include "DoDAOperation.h"
#include "DoDACase.h"

void UOperationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UOperationSubsystem::Deinitialize()
{
    Operations.Empty();
    Phases.Empty();
    Assignments.Empty();
    Outcomes.Empty();
    Super::Deinitialize();
}

FOperationId UOperationSubsystem::AllocateOperationId()
{
    FOperationId Id; Id.Value = NextOperationId++; return Id;
}

FOpPhaseId UOperationSubsystem::AllocatePhaseId()
{
    FOpPhaseId Id; Id.Value = NextPhaseId++; return Id;
}

FOperationId UOperationSubsystem::CreateOperation(FCaseId CaseId, EOperationType Type,
    EOperationPriority Priority, FRoomId LocationId)
{
    FOperationId Id = AllocateOperationId();
    FOperationRecord Rec;
    Rec.OperationId = Id;
    Rec.CaseId = CaseId;
    Rec.OperationType = Type;
    Rec.Priority = Priority;
    Rec.LocationId = LocationId;
    Rec.Status = EOperationStatus::Proposed;
    Operations.Add(Id, Rec);
    return Id;
}

FOperationRecord* UOperationSubsystem::GetOperationMutable(FOperationId Id)
{
    return Operations.Find(Id);
}

const FOperationRecord* UOperationSubsystem::GetOperation(FOperationId Id) const
{
    return Operations.Find(Id);
}

void UOperationSubsystem::SetOperationStatus(FOperationId Id, EOperationStatus NewStatus)
{
    if (FOperationRecord* Rec = Operations.Find(Id))
    {
        Rec->Status = NewStatus;
        OnOperationStatusChanged.Broadcast(Id, NewStatus);
    }
}

FOpPhaseId UOperationSubsystem::AddPhase(FOperationId OpId, const FString& PhaseName)
{
    FOpPhaseId PhaseId = AllocatePhaseId();
    FOperationPhase Phase;
    Phase.PhaseId = PhaseId;
    Phase.OperationId = OpId;
    Phase.PhaseName = PhaseName;
    Phases.Add(PhaseId, Phase);
    return PhaseId;
}

TArray<FOperationPhase> UOperationSubsystem::GetPhasesForOperation(FOperationId OpId) const
{
    TArray<FOperationPhase> Out;
    for (auto& KV : Phases)
    {
        if (KV.Value.OperationId == OpId) Out.Add(KV.Value);
    }
    return Out;
}

void UOperationSubsystem::AssignAgent(FOperationId OpId, FPawnId AgentId,
    ERoleClass Role, float Time)
{
    FOperationAssignment A;
    A.OperationId = OpId;
    A.AgentId = AgentId;
    A.AgentRole = Role;
    A.AssignedTime = Time;
    Assignments.Add(A);
}

TArray<FOperationAssignment> UOperationSubsystem::GetAssignmentsForOperation(FOperationId OpId) const
{
    TArray<FOperationAssignment> Out;
    for (const auto& A : Assignments)
    {
        if (A.OperationId == OpId) Out.Add(A);
    }
    return Out;
}

TArray<FOperationAssignment> UOperationSubsystem::GetAssignmentsForAgent(FPawnId AgentId) const
{
    TArray<FOperationAssignment> Out;
    for (const auto& A : Assignments)
    {
        if (A.AgentId == AgentId) Out.Add(A);
    }
    return Out;
}

void UOperationSubsystem::RecordOutcome(FOperationId OpId, const FOperationOutcome& Outcome)
{
    Outcomes.Add(OpId, Outcome);
}

const FOperationOutcome* UOperationSubsystem::GetOutcome(FOperationId OpId) const
{
    return Outcomes.Find(OpId);
}

TArray<FOperationRecord> UOperationSubsystem::GetOperationsForCase(FCaseId CaseId) const
{
    TArray<FOperationRecord> Out;
    for (auto& KV : Operations)
    {
        if (KV.Value.CaseId == CaseId) Out.Add(KV.Value);
    }
    return Out;
}

TArray<FOperationRecord> UOperationSubsystem::GetActiveOperations() const
{
    TArray<FOperationRecord> Out;
    for (auto& KV : Operations)
    {
        if (KV.Value.Status == EOperationStatus::Active) Out.Add(KV.Value);
    }
    return Out;
}

TArray<FOperationRecord> UOperationSubsystem::GetOperationsByType(EOperationType Type) const
{
    TArray<FOperationRecord> Out;
    for (auto& KV : Operations)
    {
        if (KV.Value.OperationType == Type) Out.Add(KV.Value);
    }
    return Out;
}