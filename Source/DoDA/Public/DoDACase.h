// Source/DoDA/Public/DoDACase.h
// Batch 05 -- Case, Phase, Task, Assignment, Evidence, Warrant structs
// and UCaseSubsystem.
// ASCII-only.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDACase.generated.h"


// -----------------------------------------------------------------
// ADDITIONAL IDs local to case system
// -----------------------------------------------------------------

USTRUCT(BlueprintType)
struct DODA_API FCasePhaseId
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = INDEX_NONE;
    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FCasePhaseId& O) const { return Value == O.Value; }
    bool operator!=(const FCasePhaseId& O) const { return Value != O.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FCasePhaseId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct DODA_API FEvidenceId
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = INDEX_NONE;
    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FEvidenceId& O) const { return Value == O.Value; }
    bool operator!=(const FEvidenceId& O) const { return Value != O.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FEvidenceId& Id) { return GetTypeHash(Id.Value); }

USTRUCT(BlueprintType)
struct DODA_API FWarrantId
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = INDEX_NONE;
    bool IsValid() const { return Value != INDEX_NONE; }
    bool operator==(const FWarrantId& O) const { return Value == O.Value; }
    bool operator!=(const FWarrantId& O) const { return Value != O.Value; }
};
FORCEINLINE uint32 GetTypeHash(const FWarrantId& Id) { return GetTypeHash(Id.Value); }


// -----------------------------------------------------------------
// ENUMS local to case system
// -----------------------------------------------------------------

UENUM(BlueprintType)
enum class EEvidenceType : uint8
{
    Physical        UMETA(DisplayName = "Physical"),
    Digital         UMETA(DisplayName = "Digital"),
    Testimonial     UMETA(DisplayName = "Testimonial"),
    Forensic        UMETA(DisplayName = "Forensic"),
    Paranormal      UMETA(DisplayName = "Paranormal"),
    Documentary     UMETA(DisplayName = "Documentary"),
};

UENUM(BlueprintType)
enum class EEvidenceStatus : uint8
{
    Uncollected     UMETA(DisplayName = "Uncollected"),
    Collected       UMETA(DisplayName = "Collected"),
    Analyzed        UMETA(DisplayName = "Analyzed"),
    Submitted       UMETA(DisplayName = "Submitted"),
    Contaminated    UMETA(DisplayName = "Contaminated"),
};

UENUM(BlueprintType)
enum class EWarrantStatus : uint8
{
    Pending         UMETA(DisplayName = "Pending"),
    Approved        UMETA(DisplayName = "Approved"),
    Denied          UMETA(DisplayName = "Denied"),
    Executed        UMETA(DisplayName = "Executed"),
    Expired         UMETA(DisplayName = "Expired"),
};

UENUM(BlueprintType)
enum class EWarrantType : uint8
{
    Search          UMETA(DisplayName = "Search"),
    Arrest          UMETA(DisplayName = "Arrest"),
    Surveillance    UMETA(DisplayName = "Surveillance"),
    Containment     UMETA(DisplayName = "Containment"),
};

UENUM(BlueprintType)
enum class EAssignmentOutcome : uint8
{
    Pending         UMETA(DisplayName = "Pending"),
    Success         UMETA(DisplayName = "Success"),
    Failure         UMETA(DisplayName = "Failure"),
    Partial         UMETA(DisplayName = "Partial"),
    Abandoned       UMETA(DisplayName = "Abandoned"),
};


// -----------------------------------------------------------------
// STRUCTS
// -----------------------------------------------------------------

USTRUCT(BlueprintType)
struct DODA_API FCase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECaseStatus Status = ECaseStatus::Open;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Difficulty = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Seed = 0;

    bool IsValid() const { return CaseId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FCasePhase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCasePhaseId PhaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPhaseType PhaseType = EPhaseType::Intake;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndTime = 0.f;

    bool IsValid() const { return PhaseId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTaskId TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCasePhaseId PhaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETaskType TaskType = ETaskType::Paperwork;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId LocationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETaskStatus Status = ETaskStatus::Pending;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAgents = 1;

    bool IsValid() const { return TaskId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FAssignment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAssignmentId AssignmentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTaskId TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId AgentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAssignmentOutcome Outcome = EAssignmentOutcome::Pending;

    bool IsValid() const { return AssignmentId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FEvidence
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEvidenceId EvidenceId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEvidenceType EvidenceType = EEvidenceType::Physical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEvidenceStatus EvidenceStatus = EEvidenceStatus::Uncollected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId LocationId;

    bool IsValid() const { return EvidenceId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FWarrant
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWarrantId WarrantId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWarrantType WarrantType = EWarrantType::Search;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWarrantStatus WarrantStatus = EWarrantStatus::Pending;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IssuedTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExpiryTime = 0.f;

    bool IsValid() const { return WarrantId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FCaseTrack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Justice = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Paranormal = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PublicTrust = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Research = 0.f;
};


// -----------------------------------------------------------------
// SUBSYSTEM
// -----------------------------------------------------------------

UCLASS()
class DODA_API UCaseSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Cases
    FCaseId CreateCase(const FString& Title, int32 Difficulty, int32 Seed);
    FCase* GetCaseMutable(FCaseId Id);
    const FCase* GetCase(FCaseId Id) const;

    // Phases
    FCasePhaseId AddPhase(FCaseId CaseId, EPhaseType PhaseType, float StartTime);
    TArray<FCasePhase> GetPhasesForCase(FCaseId CaseId) const;

    // Tasks
    FTaskId AddTask(FCaseId CaseId, FCasePhaseId PhaseId, ETaskType Type, int32 Priority);
    TArray<FTask> GetActiveTasksForCase(FCaseId CaseId) const;
    TArray<FTask> GetAllActiveTasks() const;
    FTask* GetTaskMutable(FTaskId Id);

    // Assignments
    FAssignmentId AddAssignment(FTaskId TaskId, FPawnId AgentId, float Cost, float StartTime, float EndTime);
    TArray<FAssignment> GetAssignmentsForTask(FTaskId TaskId) const;

    // Evidence
    FEvidenceId AddEvidence(FCaseId CaseId, const FString& Desc, EEvidenceType Type, FRoomId LocationId);
    TArray<FEvidence> GetEvidenceForCase(FCaseId CaseId) const;
    FEvidence* GetEvidenceMutable(FEvidenceId Id);

    // Warrants
    FWarrantId AddWarrant(FCaseId CaseId, EWarrantType Type, const FString& Target, float IssuedTime, float ExpiryTime);
    TArray<FWarrant> GetWarrantsForCase(FCaseId CaseId) const;
    FWarrant* GetWarrantMutable(FWarrantId Id);

    // Tracks
    FCaseTrack* GetTrackMutable(FCaseId CaseId);
    const FCaseTrack* GetTrack(FCaseId CaseId) const;

    const TMap<FCaseId, FCase>& GetAllCases() const { return Cases; }

private:
    TMap<FCaseId, FCase>             Cases;
    TMap<FCasePhaseId, FCasePhase>   Phases;
    TMap<FTaskId, FTask>             Tasks;
    TMap<FAssignmentId, FAssignment> Assignments;
    TMap<FEvidenceId, FEvidence>     Evidences;
    TMap<FWarrantId, FWarrant>       Warrants;
    TMap<FCaseId, FCaseTrack>        Tracks;

    int32 NextCaseId = 1;
    int32 NextPhaseId = 1;
    int32 NextTaskId = 1;
    int32 NextAssignId = 1;
    int32 NextEvidenceId = 1;
    int32 NextWarrantId = 1;

    FCaseId       AllocateCaseId();
    FCasePhaseId  AllocatePhaseId();
    FTaskId       AllocateTaskId();
    FAssignmentId AllocateAssignmentId();
    FEvidenceId   AllocateEvidenceId();
    FWarrantId    AllocateWarrantId();
};