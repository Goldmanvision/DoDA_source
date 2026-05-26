#include "DoDACult.h"
#include "DoDACase.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void UCultSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    SeedTestData();
}

void UCultSubsystem::Deinitialize()
{
    Cells.Empty();
    Members.Empty();
    AreaInfluences.Empty();
    Entities.Empty();
    Sites.Empty();
    Super::Deinitialize();
}

// -----------------------------------------------------------------
// ALLOCATION
// -----------------------------------------------------------------

FCultCellId UCultSubsystem::AllocateCellId()
{
    FCultCellId Id; Id.Value = NextCellId++; return Id;
}

FEntityId UCultSubsystem::AllocateEntityId()
{
    FEntityId Id; Id.Value = NextEntityId++; return Id;
}

FParanormalSiteId UCultSubsystem::AllocateSiteId()
{
    FParanormalSiteId Id; Id.Value = NextSiteId++; return Id;
}

// -----------------------------------------------------------------
// CULT CELLS
// -----------------------------------------------------------------

FCultCellId UCultSubsystem::AddCultCell(const FString& Name, FAreaId AreaId, FCaseId CaseId)
{
    FCultCellId NewId = AllocateCellId();
    FCultCell Cell;
    Cell.CellId = NewId;
    Cell.CellName = Name;
    Cell.AreaId = AreaId;
    Cell.LinkedCaseId = CaseId;
    Cell.Influence = 10.f;
    Cell.ThreatState = EHazardState::Latent;
    Cells.Add(NewId, Cell);
    EnsureAreaInfluence(AreaId);
    return NewId;
}

FCultCell* UCultSubsystem::GetCellMutable(FCultCellId Id)
{
    return Cells.Find(Id);
}

const FCultCell* UCultSubsystem::GetCell(FCultCellId Id) const
{
    return Cells.Find(Id);
}

TArray<FCultCell> UCultSubsystem::GetAllCells() const
{
    TArray<FCultCell> Out;
    Cells.GenerateValueArray(Out);
    return Out;
}

// -----------------------------------------------------------------
// CULT MEMBERS
// -----------------------------------------------------------------

void UCultSubsystem::AddCultMember(FPawnId PawnId, FCultCellId CellId, ECultRoleInCell Role)
{
    FCultMember Member;
    Member.PawnId = PawnId;
    Member.CellId = CellId;
    Member.RoleInCell = Role;
    Members.Add(PawnId, Member);
}

TArray<FCultMember> UCultSubsystem::GetMembersForCell(FCultCellId CellId) const
{
    TArray<FCultMember> Out;
    for (const TPair<FPawnId, FCultMember>& Pair : Members)
    {
        if (Pair.Value.CellId == CellId)
            Out.Add(Pair.Value);
    }
    return Out;
}

// -----------------------------------------------------------------
// AREA INFLUENCE
// -----------------------------------------------------------------

void UCultSubsystem::EnsureAreaInfluence(FAreaId AreaId)
{
    if (!AreaInfluences.Contains(AreaId))
    {
        FAreaInfluence AI;
        AI.AreaId = AreaId;
        AreaInfluences.Add(AreaId, AI);
    }
}

FAreaInfluence* UCultSubsystem::GetAreaInfluenceMutable(FAreaId AreaId)
{
    return AreaInfluences.Find(AreaId);
}

const FAreaInfluence* UCultSubsystem::GetAreaInfluence(FAreaId AreaId) const
{
    return AreaInfluences.Find(AreaId);
}

// -----------------------------------------------------------------
// PARANORMAL ENTITIES
// -----------------------------------------------------------------

FEntityId UCultSubsystem::AddEntity(const FString& Name, EParanormalType Type, float ThreatRating)
{
    FEntityId NewId = AllocateEntityId();
    FParanormalEntity Entity;
    Entity.EntityId = NewId;
    Entity.Name = Name;
    Entity.EntityType = Type;
    Entity.ThreatRating = ThreatRating;
    Entities.Add(NewId, Entity);
    return NewId;
}

const FParanormalEntity* UCultSubsystem::GetEntity(FEntityId Id) const
{
    return Entities.Find(Id);
}

// -----------------------------------------------------------------
// PARANORMAL SITES
// -----------------------------------------------------------------

FParanormalSiteId UCultSubsystem::AddSite(FEntityId EntityId, FAreaId AreaId, FRoomId RoomId)
{
    FParanormalSiteId NewId = AllocateSiteId();
    FParanormalSite Site;
    Site.SiteId = NewId;
    Site.EntityId = EntityId;
    Site.AreaId = AreaId;
    Site.RoomId = RoomId;
    Site.SiteState = EHazardState::None;
    Site.Intensity = 5.f;
    Sites.Add(NewId, Site);
    EnsureAreaInfluence(AreaId);
    return NewId;
}

FParanormalSite* UCultSubsystem::GetSiteMutable(FParanormalSiteId Id)
{
    return Sites.Find(Id);
}

TArray<FParanormalSite> UCultSubsystem::GetSitesForArea(FAreaId AreaId) const
{
    TArray<FParanormalSite> Out;
    for (const TPair<FParanormalSiteId, FParanormalSite>& Pair : Sites)
    {
        if (Pair.Value.AreaId == AreaId)
            Out.Add(Pair.Value);
    }
    return Out;
}

// -----------------------------------------------------------------
// TICK
// -----------------------------------------------------------------

EHazardState UCultSubsystem::ComputeHazardState(float Intensity) const
{
    if (Intensity <= 0.f)  return EHazardState::None;
    if (Intensity < 25.f)  return EHazardState::Latent;
    if (Intensity < 60.f)  return EHazardState::Active;
    if (Intensity < 90.f)  return EHazardState::Critical;
    return EHazardState::Critical;
}

void UCultSubsystem::StepCult(float DeltaSimTime)
{
    for (TPair<FCultCellId, FCultCell>& Pair : Cells)
    {
        FCultCell& Cell = Pair.Value;
        if (Cell.bExposed) continue;

        int32 MemberCount = GetMembersForCell(Cell.CellId).Num();
        float GrowthRate = 0.5f + MemberCount * 0.2f;
        Cell.Influence = FMath::Clamp(Cell.Influence + GrowthRate * DeltaSimTime, 0.f, 100.f);
        Cell.ThreatState = ComputeHazardState(Cell.Influence);

        FAreaInfluence* AI = AreaInfluences.Find(Cell.AreaId);
        if (AI)
        {
            AI->CultInfluence = FMath::Clamp(AI->CultInfluence + GrowthRate * 0.5f * DeltaSimTime, 0.f, 100.f);
            AI->HazardState = ComputeHazardState(FMath::Max(AI->CultInfluence, AI->ParanormalIntensity));
        }
    }
}

void UCultSubsystem::StepParanormal(float DeltaSimTime)
{
    for (TPair<FParanormalSiteId, FParanormalSite>& Pair : Sites)
    {
        FParanormalSite& Site = Pair.Value;
        if (Site.SiteState == EHazardState::Contained) continue;

        const FParanormalEntity* Entity = Entities.Find(Site.EntityId);
        float GrowthRate = Entity ? Entity->ThreatRating * 0.3f : 0.2f;

        FAreaInfluence* AI = AreaInfluences.Find(Site.AreaId);
        float CultBoost = AI ? AI->CultInfluence * 0.01f : 0.f;

        Site.Intensity = FMath::Clamp(Site.Intensity + (GrowthRate + CultBoost) * DeltaSimTime, 0.f, 100.f);
        Site.SiteState = ComputeHazardState(Site.Intensity);

        if (AI)
        {
            AI->ParanormalIntensity = FMath::Clamp(AI->ParanormalIntensity + GrowthRate * 0.4f * DeltaSimTime, 0.f, 100.f);
            AI->HazardState = ComputeHazardState(FMath::Max(AI->CultInfluence, AI->ParanormalIntensity));
        }

        if (Site.SiteState == EHazardState::Critical)
        {
            UE_LOG(LogTemp, Warning, TEXT("DoDA|Cult -- Paranormal site %d CRITICAL in area %d"),
                Site.SiteId.Value, Site.AreaId.Value);
        }
    }
}

// -----------------------------------------------------------------
// SEED
// -----------------------------------------------------------------

void UCultSubsystem::SeedTestData()
{
    FAreaId TestArea; TestArea.Value = 1;
    FCaseId TestCase; TestCase.Value = 1;
    FRoomId TestRoom; TestRoom.Value = 1;

    FCultCellId CellId = AddCultCell(TEXT("Cell Azathoth-7"), TestArea, TestCase);

    FPawnId M1; M1.Value = 10;
    FPawnId M2; M2.Value = 11;
    AddCultMember(M1, CellId, ECultRoleInCell::Priest);
    AddCultMember(M2, CellId, ECultRoleInCell::Operative);

    FEntityId EntId = AddEntity(TEXT("Splinter of Azathoth"), EParanormalType::Dimensional, 3.f);
    AddSite(EntId, TestArea, TestRoom);
}