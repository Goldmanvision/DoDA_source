// Source/DoDA/Private/DoDAIdentity.cpp
// Batch 02 -- ASCII-only.

#include "DoDAIdentity.h"
#include "Misc/Guid.h"

void UIdentitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Records.Empty();
    GuidIndex.Empty();
    NextIdentityId = 1;
}

void UIdentitySubsystem::Deinitialize()
{
    Records.Empty();
    GuidIndex.Empty();
    Super::Deinitialize();
}

FIdentityId UIdentitySubsystem::AllocateIdentityId()
{
    FIdentityId Id;
    Id.Value = NextIdentityId++;
    return Id;
}

FIdentityId UIdentitySubsystem::RegisterIdentity(FIdentityRecord& Record)
{
    if (!Record.WorldGuid.IsValid())
    {
        Record.WorldGuid = FGuid::NewGuid();
    }

    if (!Record.IdentityId.IsValid())
    {
        Record.IdentityId = AllocateIdentityId();
    }

    if (const FIdentityRecord* Existing = Records.Find(Record.IdentityId))
    {
        if (Existing->WorldGuid.IsValid())
        {
            GuidIndex.Remove(Existing->WorldGuid);
        }
    }

    Records.Add(Record.IdentityId, Record);

    if (Record.WorldGuid.IsValid())
    {
        GuidIndex.Add(Record.WorldGuid, Record.IdentityId);
    }

    return Record.IdentityId;
}

void UIdentitySubsystem::UnregisterIdentity(FIdentityId Id)
{
    if (const FIdentityRecord* Record = Records.Find(Id))
    {
        if (Record->WorldGuid.IsValid())
        {
            GuidIndex.Remove(Record->WorldGuid);
        }

        Records.Remove(Id);
    }
}

const FIdentityRecord* UIdentitySubsystem::GetById(FIdentityId Id) const
{
    return Records.Find(Id);
}

const FIdentityRecord* UIdentitySubsystem::GetByGuid(const FGuid& WorldGuid) const
{
    if (const FIdentityId* FoundId = GuidIndex.Find(WorldGuid))
    {
        return Records.Find(*FoundId);
    }

    return nullptr;
}

void UIdentitySubsystem::SeedTestIdentities(int32 Count)
{
    static const TArray<FString> Names = {
        TEXT("Alan Marsh"), TEXT("Dana Voss"), TEXT("Eli Crane"),
        TEXT("Fiona Hale"), TEXT("Greg Tully"), TEXT("Helen Cross"),
        TEXT("Ivan Rook"), TEXT("Janet Farr"), TEXT("Karl Webb"),
        TEXT("Laura Stone")
    };

    static const TArray<FString> Jobs = {
        TEXT("Field Agent"), TEXT("Analyst"), TEXT("Forensic Tech"),
        TEXT("Archivist"), TEXT("Negotiator"), TEXT("Administrator")
    };

    for (int32 i = 0; i < Count; ++i)
    {
        FIdentityRecord R;
        R.FullName = Names[i % Names.Num()];
        R.Age = 28 + (i % 20);
        R.Sex = (i % 2 == 0) ? TEXT("M") : TEXT("F");
        R.Address = FString::Printf(TEXT("%d Bureau Ave"), 100 + i);
        R.Phone = FString::Printf(TEXT("555-01%02d"), i);
        R.JobTitle = Jobs[i % Jobs.Num()];
        R.Employer = TEXT("Dept. of Diplomagic Affairs");
        R.Vehicle = TEXT("Unknown");
        R.BloodType = TEXT("O+");
        R.WorldGuid = FGuid::NewGuid();
        RegisterIdentity(R);
    }
}