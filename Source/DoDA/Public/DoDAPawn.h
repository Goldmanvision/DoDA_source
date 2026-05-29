// Source/DoDA/Public/DoDAPawn.h
// Batch 03 -- Pawn data structs and UPawnSubsystem.
// ASCII-only.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDAIdentity.h"
#include "DoDAPawn.generated.h"


USTRUCT(BlueprintType)
struct DODA_API FRoleState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFaction Faction = EFaction::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERoleClass RoleClass = ERoleClass::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPawnStatus CurrentStatus = EPawnStatus::Active;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWorkState WorkState = EWorkState::Available;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBuildingId HomeBaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId CurrentLocationId;
};


USTRUCT(BlueprintType)
struct DODA_API FPrimaryStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Body = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Agility = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Mind = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Will = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Presence = 5;
};


USTRUCT(BlueprintType)
struct DODA_API FVitals
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BloodCurrent = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BloodMax = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaminaCurrent = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StaminaMax = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StressCurrent = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StressMax = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SanityCurrent = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SanityMax = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FocusCurrent = 100.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FocusMax = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PainCurrent = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExhaustionCurrent = 0.f;
};


USTRUCT(BlueprintType)
struct DODA_API FSkills
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Investigation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Observation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Surveillance = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Stealth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Firearms = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 CloseCombat = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Evasion = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Arrest = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Forensics = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DigitalForensics = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Engineering = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Medicine = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Persuasion = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Interviewing = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Bureaucracy = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Command = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 OccultKnowledge = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 FieldResearch = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Containment = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DiplomagicalNegotiation = 0;

    int32 GetSkillForTask(ETaskType TaskType) const;
};


USTRUCT(BlueprintType)
struct DODA_API FPawnRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId PawnId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIdentityId IdentityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoleState RoleState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPrimaryStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVitals Vitals;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSkills Skills;

    bool IsValid() const { return PawnId.IsValid(); }
};


UCLASS()
class DODA_API UPawnSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void SeedTestPawns(int32 Count);
    int32 Count() const { return Pawns.Num(); }

    void RegisterPawn(const FPawnRecord& Record);
    void UnregisterPawn(FPawnId Id);

    FPawnRecord* GetPawnMutable(FPawnId Id);
    const FPawnRecord* GetPawn(FPawnId Id) const;
    const TMap<FPawnId, FPawnRecord>& GetAllPawns() const { return Pawns; }

    void UpdateVitals(float DeltaTime);
    void ApplyVitalsDelta(FPawnId Id, float StaminaDelta, float StressDelta, float SanityDelta);

private:
    TMap<FPawnId, FPawnRecord> Pawns;
    int32 NextPawnId = 1;

    FPawnId AllocatePawnId();
};