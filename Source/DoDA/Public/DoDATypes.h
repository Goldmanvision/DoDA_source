// Fill out your copyright notice in the Description page of Project Settings.

// Source/DoDA/Public/DoDATypes.h
// Batch 01 — Core typed IDs and enums for DoDA simulation.
// No gameplay logic. Include this in every subsystem header.

#pragma once

#include "CoreMinimal.h"
#include "DoDATypes.generated.h"

// ─────────────────────────────────────────────────────────────
// TYPED ID STRUCTS
// Each system gets its own opaque ID type.
// Use FPawnId instead of raw int32 everywhere.
// ─────────────────────────────────────────────────────────────

USTRUCT(BlueprintType)
struct DODA_API FPawnId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FPawnId& Other) const { return Value == Other.Value; }
    bool operator!=(const FPawnId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FPawnId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FCaseId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FCaseId& Other) const { return Value == Other.Value; }
    bool operator!=(const FCaseId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FCaseId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FTaskId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FTaskId& Other) const { return Value == Other.Value; }
    bool operator!=(const FTaskId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FTaskId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FAssignmentId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FAssignmentId& Other) const { return Value == Other.Value; }
    bool operator!=(const FAssignmentId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAssignmentId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FAreaId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FAreaId& Other) const { return Value == Other.Value; }
    bool operator!=(const FAreaId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FAreaId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FBuildingId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FBuildingId& Other) const { return Value == Other.Value; }
    bool operator!=(const FBuildingId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FBuildingId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FRoomId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FRoomId& Other) const { return Value == Other.Value; }
    bool operator!=(const FRoomId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FRoomId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FTownId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FTownId& Other) const { return Value == Other.Value; }
    bool operator!=(const FTownId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FTownId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FBlockId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FBlockId& Other) const { return Value == Other.Value; }
    bool operator!=(const FBlockId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FBlockId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FLotId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FLotId& Other) const { return Value == Other.Value; }
    bool operator!=(const FLotId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FLotId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FCultCellId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FCultCellId& Other) const { return Value == Other.Value; }
    bool operator!=(const FCultCellId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FCultCellId& Id) { return GetTypeHash(Id.Value); }


USTRUCT(BlueprintType)
struct DODA_API FEntityId
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Value = INDEX_NONE;

    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FEntityId& Other) const { return Value == Other.Value; }
    bool operator!=(const FEntityId& Other) const { return Value != Other.Value; }
};

FORCEINLINE uint32 GetTypeHash(const FEntityId& Id) { return GetTypeHash(Id.Value); }


// ─────────────────────────────────────────────────────────────
// ENUMS
// ─────────────────────────────────────────────────────────────

UENUM(BlueprintType)
enum class ERoleClass : uint8
{
    None            UMETA(DisplayName = "None"),
    FieldAgent      UMETA(DisplayName = "Field Agent"),
    Analyst         UMETA(DisplayName = "Analyst"),
    Forensics       UMETA(DisplayName = "Forensics"),
    Negotiator      UMETA(DisplayName = "Negotiator"),
    Commander       UMETA(DisplayName = "Commander"),
    Administrator   UMETA(DisplayName = "Administrator"),
    Informant       UMETA(DisplayName = "Informant"),
    Civilian        UMETA(DisplayName = "Civilian"),
    CultMember      UMETA(DisplayName = "Cult Member"),
    Unknown         UMETA(DisplayName = "Unknown"),
};


UENUM(BlueprintType)
enum class EFaction : uint8
{
    None            UMETA(DisplayName = "None"),
    Bureau          UMETA(DisplayName = "Bureau"),
    Police          UMETA(DisplayName = "Police"),
    Military        UMETA(DisplayName = "Military"),
    Civilian        UMETA(DisplayName = "Civilian"),
    Cult            UMETA(DisplayName = "Cult"),
    Unknown         UMETA(DisplayName = "Unknown"),
};


UENUM(BlueprintType)
enum class EPawnStatus : uint8
{
    Active          UMETA(DisplayName = "Active"),
    Incapacitated   UMETA(DisplayName = "Incapacitated"),
    Dead            UMETA(DisplayName = "Dead"),
    Missing         UMETA(DisplayName = "Missing"),
    Detained        UMETA(DisplayName = "Detained"),
    Rogue           UMETA(DisplayName = "Rogue"),
};


UENUM(BlueprintType)
enum class ECaseStatus : uint8
{
    Open            UMETA(DisplayName = "Open"),
    Active          UMETA(DisplayName = "Active"),
    Closed          UMETA(DisplayName = "Closed"),
    Shelved         UMETA(DisplayName = "Shelved"),
    Classified      UMETA(DisplayName = "Classified"),
    Compromised     UMETA(DisplayName = "Compromised"),
};


UENUM(BlueprintType)
enum class EPhaseType : uint8
{
    Intake          UMETA(DisplayName = "Intake"),
    Investigation   UMETA(DisplayName = "Investigation"),
    Warrant         UMETA(DisplayName = "Warrant"),
    Enforcement     UMETA(DisplayName = "Enforcement"),
    Resolution      UMETA(DisplayName = "Resolution"),
    Paranormal      UMETA(DisplayName = "Paranormal"),
};


UENUM(BlueprintType)
enum class ETaskType : uint8
{
    Surveillance    UMETA(DisplayName = "Surveillance"),
    Interview       UMETA(DisplayName = "Interview"),
    ForensicSweep   UMETA(DisplayName = "Forensic Sweep"),
    Research        UMETA(DisplayName = "Research"),
    Arrest          UMETA(DisplayName = "Arrest"),
    Containment     UMETA(DisplayName = "Containment"),
    Paperwork       UMETA(DisplayName = "Paperwork"),
    Negotiation     UMETA(DisplayName = "Negotiation"),
    Infiltration    UMETA(DisplayName = "Infiltration"),
    ParanormalScan  UMETA(DisplayName = "Paranormal Scan"),
};


UENUM(BlueprintType)
enum class ETaskStatus : uint8
{
    Pending         UMETA(DisplayName = "Pending"),
    Assigned        UMETA(DisplayName = "Assigned"),
    InProgress      UMETA(DisplayName = "In Progress"),
    Complete        UMETA(DisplayName = "Complete"),
    Failed          UMETA(DisplayName = "Failed"),
    Blocked         UMETA(DisplayName = "Blocked"),
};


UENUM(BlueprintType)
enum class EHazardState : uint8
{
    None            UMETA(DisplayName = "None"),
    Latent          UMETA(DisplayName = "Latent"),
    Active          UMETA(DisplayName = "Active"),
    Critical        UMETA(DisplayName = "Critical"),
    Contained       UMETA(DisplayName = "Contained"),
};


UENUM(BlueprintType)
enum class EParanormalType : uint8
{
    Unknown         UMETA(DisplayName = "Unknown"),
    Psychic         UMETA(DisplayName = "Psychic"),
    Possession      UMETA(DisplayName = "Possession"),
    Dimensional     UMETA(DisplayName = "Dimensional"),
    Biological      UMETA(DisplayName = "Biological"),
    Memetic         UMETA(DisplayName = "Memetic"),
};


UENUM(BlueprintType)
enum class EZoningType : uint8
{
    Residential     UMETA(DisplayName = "Residential"),
    Commercial      UMETA(DisplayName = "Commercial"),
    Industrial      UMETA(DisplayName = "Industrial"),
    Government      UMETA(DisplayName = "Government"),
    Abandoned       UMETA(DisplayName = "Abandoned"),
    Rural           UMETA(DisplayName = "Rural"),
};


UENUM(BlueprintType)
enum class EBuildingType : uint8
{
    House           UMETA(DisplayName = "House"),
    Apartment       UMETA(DisplayName = "Apartment"),
    Office          UMETA(DisplayName = "Office"),
    Warehouse       UMETA(DisplayName = "Warehouse"),
    Church          UMETA(DisplayName = "Church"),
    Government      UMETA(DisplayName = "Government"),
    Commercial      UMETA(DisplayName = "Commercial"),
    Industrial      UMETA(DisplayName = "Industrial"),
    Abandoned       UMETA(DisplayName = "Abandoned"),
};


UENUM(BlueprintType)
enum class ERoomType : uint8
{
    Generic         UMETA(DisplayName = "Generic"),
    Office          UMETA(DisplayName = "Office"),
    Hallway         UMETA(DisplayName = "Hallway"),
    Basement        UMETA(DisplayName = "Basement"),
    Bedroom         UMETA(DisplayName = "Bedroom"),
    Kitchen         UMETA(DisplayName = "Kitchen"),
    Lab             UMETA(DisplayName = "Lab"),
    StorageRoom     UMETA(DisplayName = "Storage Room"),
    Vault           UMETA(DisplayName = "Vault"),
};


UENUM(BlueprintType)
enum class ECheckType : uint8
{
    Investigation   UMETA(DisplayName = "Investigation"),
    Persuasion      UMETA(DisplayName = "Persuasion"),
    Medicine        UMETA(DisplayName = "Medicine"),
    SanityCheck     UMETA(DisplayName = "Sanity Check"),
    Stealth         UMETA(DisplayName = "Stealth"),
    Combat          UMETA(DisplayName = "Combat"),
    Occult          UMETA(DisplayName = "Occult"),
};


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Unarmed         UMETA(DisplayName = "Unarmed"),
    Pistol          UMETA(DisplayName = "Pistol"),
    Shotgun         UMETA(DisplayName = "Shotgun"),
    Rifle           UMETA(DisplayName = "Rifle"),
    Knife           UMETA(DisplayName = "Knife"),
    Explosive       UMETA(DisplayName = "Explosive"),
    Paranormal      UMETA(DisplayName = "Paranormal"),
};


UENUM(BlueprintType)
enum class ECultRoleInCell : uint8
{
    Initiate        UMETA(DisplayName = "Initiate"),
    Operative       UMETA(DisplayName = "Operative"),
    Acolyte         UMETA(DisplayName = "Acolyte"),
    Priest          UMETA(DisplayName = "Priest"),
    Handler         UMETA(DisplayName = "Handler"),
};
