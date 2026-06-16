#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDACase.h"
#include "DoDACult.generated.h"

// -----------------------------------------------------------------
// ADDITIONAL IDs
// -----------------------------------------------------------------

USTRUCT(BlueprintType)
struct DODA_API FParanormalSiteId
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = INDEX_NONE;
    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FParanormalSiteId& O) const { return Value == O.Value; }
    bool operator!=(const FParanormalSiteId& O) const { return Value != O.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FParanormalSiteId& Id) { return GetTypeHash(Id.Value); }


// -----------------------------------------------------------------
// STRUCTS
// -----------------------------------------------------------------

USTRUCT(BlueprintType)
struct DODA_API FCultCell
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCultCellId CellId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId LinkedCaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAreaId AreaId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CellName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Influence = 0.f;            // 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHazardState ThreatState = EHazardState::Latent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bExposed = false;

    bool IsValid() const { return CellId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FCultMember
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId PawnId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCultCellId CellId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECultRoleInCell RoleInCell = ECultRoleInCell::Initiate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIdentified = false;

    bool IsValid() const { return PawnId.IsValid() && CellId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FAreaInfluence
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAreaId AreaId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CultInfluence = 0.f;        // 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ParanormalIntensity = 0.f;  // 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHazardState HazardState = EHazardState::None;
};


USTRUCT(BlueprintType)
struct DODA_API FParanormalEntity
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEntityId EntityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EParanormalType EntityType = EParanormalType::Unknown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatRating = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Notes;

    bool IsValid() const { return EntityId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FParanormalSite
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FParanormalSiteId SiteId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEntityId EntityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAreaId AreaId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId RoomId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHazardState SiteState = EHazardState::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intensity = 0.f;            // 0-100

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDiscovered = false;

    bool IsValid() const { return SiteId.IsValid(); }
};


// -----------------------------------------------------------------
// SUBSYSTEM
// -----------------------------------------------------------------

UCLASS()
class DODA_API UCultSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Cult cells
    FCultCellId             AddCultCell(const FString& Name, FAreaId AreaId, FCaseId CaseId);
    FCultCell* GetCellMutable(FCultCellId Id);
    const FCultCell* GetCell(FCultCellId Id) const;
    TArray<FCultCell>       GetAllCells() const;

    // Cult members
    void                    AddCultMember(FPawnId PawnId, FCultCellId CellId, ECultRoleInCell Role);
    TArray<FCultMember>     GetMembersForCell(FCultCellId CellId) const;

    // Area influence
    FAreaInfluence* GetAreaInfluenceMutable(FAreaId AreaId);
    const FAreaInfluence* GetAreaInfluence(FAreaId AreaId) const;

    // Paranormal entities
    FEntityId               AddEntity(const FString& Name, EParanormalType Type, float ThreatRating);
    const FParanormalEntity* GetEntity(FEntityId Id) const;

    // Paranormal sites
    FParanormalSiteId       AddSite(FEntityId EntityId, FAreaId AreaId, FRoomId RoomId);
    FParanormalSite* GetSiteMutable(FParanormalSiteId Id);
    TArray<FParanormalSite> GetSitesForArea(FAreaId AreaId) const;

    // Tick -- step influence and intensity, escalate hazard states
    void StepCult(float DeltaSimTime);
    void StepParanormal(float DeltaSimTime);

    // Seed for testing
    void SeedTestData();

private:
    TMap<FCultCellId, FCultCell>          Cells;
    TMap<FPawnId, FCultMember>        Members;
    TMap<FAreaId, FAreaInfluence>     AreaInfluences;
    TMap<FEntityId, FParanormalEntity>  Entities;
    TMap<FParanormalSiteId, FParanormalSite>    Sites;

    int32 NextCellId = 1;
    int32 NextEntityId = 1;
    int32 NextSiteId = 1;

    FCultCellId       AllocateCellId();
    FEntityId         AllocateEntityId();
    FParanormalSiteId AllocateSiteId();

    EHazardState ComputeHazardState(float Intensity) const;
    void         EnsureAreaInfluence(FAreaId AreaId);
};