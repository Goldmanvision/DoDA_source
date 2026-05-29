// Source/DoDA/Private/DoDAPawn.cpp
// Batch 03 -- ASCII-only.

#include "DoDAPawn.h"
#include "DoDAIdentity.h"

int32 FSkills::GetSkillForTask(ETaskType TaskType) const
{
    switch (TaskType)
    {
    case ETaskType::Surveillance:   return Surveillance;
    case ETaskType::Interview:      return Interviewing;
    case ETaskType::ForensicSweep:  return Forensics;
    case ETaskType::Research:       return FieldResearch;
    case ETaskType::Arrest:         return Arrest;
    case ETaskType::Containment:     return Containment;
    case ETaskType::Paperwork:       return Bureaucracy;
    case ETaskType::Negotiation:     return DiplomagicalNegotiation;
    case ETaskType::Infiltration:    return Stealth;
    case ETaskType::ParanormalScan:  return OccultKnowledge;
    default:                         return 0;
    }
}

void UPawnSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Pawns.Empty();
    NextPawnId = 1;
}

void UPawnSubsystem::Deinitialize()
{
    Pawns.Empty();
    Super::Deinitialize();
}

FPawnId UPawnSubsystem::AllocatePawnId()
{
    FPawnId Id;
    Id.Value = NextPawnId++;
    return Id;
}

void UPawnSubsystem::RegisterPawn(const FPawnRecord& Record)
{
    if (!Record.IsValid()) return;
    Pawns.Add(Record.PawnId, Record);
}

void UPawnSubsystem::UnregisterPawn(FPawnId Id)
{
    Pawns.Remove(Id);
}

FPawnRecord* UPawnSubsystem::GetPawnMutable(FPawnId Id)
{
    return Pawns.Find(Id);
}

const FPawnRecord* UPawnSubsystem::GetPawn(FPawnId Id) const
{
    return Pawns.Find(Id);
}

void UPawnSubsystem::UpdateVitals(float DeltaTime)
{
    for (auto& Pair : Pawns)
    {
        FPawnRecord& P = Pair.Value;
        if (P.RoleState.CurrentStatus != EPawnStatus::Active) continue;

        FVitals& V = P.Vitals;

        V.StaminaCurrent = FMath::Max(0.f, V.StaminaCurrent - (1.f * DeltaTime));

        if (V.StaminaCurrent <= 0.f)
        {
            V.ExhaustionCurrent = FMath::Min(100.f, V.ExhaustionCurrent + (2.f * DeltaTime));
            P.RoleState.WorkState = EWorkState::Recovering;
        }

        if (V.StressCurrent < 25.f)
        {
            V.FocusCurrent = FMath::Min(V.FocusMax, V.FocusCurrent + (0.5f * DeltaTime));
        }

        V.PainCurrent = FMath::Max(0.f, V.PainCurrent - (0.5f * DeltaTime));

        if (V.BloodCurrent <= 0.f || V.SanityCurrent <= 0.f)
        {
            P.RoleState.CurrentStatus = EPawnStatus::Incapacitated;
            P.RoleState.WorkState = EWorkState::Injured;
        }
    }
}

void UPawnSubsystem::ApplyVitalsDelta(FPawnId Id, float StaminaDelta, float StressDelta, float SanityDelta)
{
    FPawnRecord* Pawn = GetPawnMutable(Id);
    if (!Pawn) return;

    FVitals& V = Pawn->Vitals;
    V.StaminaCurrent = FMath::Clamp(V.StaminaCurrent + StaminaDelta, 0.f, V.StaminaMax);
    V.StressCurrent = FMath::Clamp(V.StressCurrent + StressDelta, 0.f, V.StressMax);
    V.SanityCurrent = FMath::Clamp(V.SanityCurrent + SanityDelta, 0.f, V.SanityMax);
}

void UPawnSubsystem::SeedTestPawns(int32 Count)
{
    UIdentitySubsystem* IdSys = GetWorld()->GetSubsystem<UIdentitySubsystem>();
    if (!IdSys) return;

    int32 Seeded = 0;
    for (const auto& Pair : IdSys->GetAll())
    {
        if (Seeded >= Count) break;

        const FIdentityRecord& Identity = Pair.Value;

        FPawnRecord R;
        R.PawnId = AllocatePawnId();
        R.IdentityId = Identity.IdentityId;

        R.RoleState.Faction = EFaction::Bureau;
        R.RoleState.RoleClass = ERoleClass::FieldAgent;
        R.RoleState.CurrentStatus = EPawnStatus::Active;
        R.RoleState.WorkState = EWorkState::Available;

        R.Stats.Body = 5;
        R.Stats.Agility = 5;
        R.Stats.Mind = 5;
        R.Stats.Will = 5;
        R.Stats.Presence = 5;

        R.Skills.Investigation = 40;
        R.Skills.Surveillance = 30;
        R.Skills.Firearms = 35;
        R.Skills.Bureaucracy = 25;
        R.Skills.Interviewing = 30;

        RegisterPawn(R);
        ++Seeded;
    }
}