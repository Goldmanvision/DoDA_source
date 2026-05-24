// Source/DoDA/Private/DoDAIdentity.cpp
// Batch 02 -- ASCII-only.

#include "DoDAIdentity.h"
#include "Misc/Guid.h"

void UIdentitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Records.Empty();
    GuidIndex.Empty();
    NextSeedId = 1;
}

void UIdentitySubsystem::Deinitialize()
{
    Records.Empty();
    GuidIndex.Empty();
    Super::Deinitialize();
}

void UIdentitySubsystem::RegisterIdentity(const FIdentityRecord& Record)
{
    if (!Record.IsValid()) return;

    if (const FIdentityRecord* Existing = Records.Find(Record.IdentityId))
    {
        GuidIndex.Remove(Existing->WorldGuid);
    }

    Records.Add(Record.IdentityId, Record);

    if (Record.WorldGuid.IsValid())
    {
        GuidIndex.Add(Record.WorldGuid, Record.IdentityId);
    }
}

void UIdentitySubsystem::UnregisterIdentity(const FString& IdentityId)
{
    if (const FIdentityRecord* Record = Records.Find(IdentityId))
    {
        GuidIndex.Remove(Record->WorldGuid);
        Records.Remove(IdentityId);
    }
}

const FIdentityRecord* UIdentitySubsystem::GetById(const FString& IdentityId) const
{
    return Records.Find(IdentityId);
}

const FIdentityRecord* UIdentitySubsystem::GetByGuid(const FGuid& WorldGuid) const
{
    if (const FString* FoundId = GuidIndex.Find(WorldGuid))
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
        R.IdentityId = FString::Printf(TEXT("ID-%04d"), NextSeedId++);
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