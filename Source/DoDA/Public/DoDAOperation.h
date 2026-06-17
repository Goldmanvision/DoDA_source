// Source/DoDA/Public/DoDAOperation.h
// Batch 08 -- Operation types and mission structure.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDACase.h"
#include "DoDAOperation.generated.h"

UENUM(BlueprintType)
enum class EOperationType : uint8
{
    Investigation   UMETA(DisplayName = "Investigation"),
    Surveillance    UMETA(DisplayName = "Surveillance"),
    Warrant         UMETA(DisplayName = "Warrant"),
    Search          UMETA(DisplayName = "Search"),
    Arrest          UMETA(DisplayName = "Arrest"),
    Murder          UMETA(DisplayName = "Murder"),
    Extortion       UMETA(DisplayName = "Extortion"),
    FinancialCrimes UMETA(DisplayName = "Financial Crimes"),
    CultCrimes      UMETA(DisplayName = "Cult Crimes"),
    RICO            UMETA(DisplayName = "RICO"),
};

UENUM(BlueprintType)
enum class EOperationStatus : uint8
{
    Proposed    UMETA(DisplayName = "Proposed"),
    Prepared    UMETA(DisplayName = "Prepared"),
    Active      UMETA(DisplayName = "Active"),
    Delayed     UMETA(DisplayName = "Delayed"),
    Failed      UMETA(DisplayName = "Failed"),
    Completed   UMETA(DisplayName = "Completed"),
    Escalated   UMETA(DisplayName = "Escalated"),
};

UENUM(BlueprintType)
enum class EOperationRisk : uint8
{
    Low      UMETA(DisplayName = "Low"),
    Moderate UMETA(DisplayName = "Moderate"),
    High     UMETA(DisplayName = "High"),
    Extreme  UMETA(DisplayName = "Extreme"),
};

UENUM(BlueprintType)
enum class EOperationPriority : uint8
{
    Routine   UMETA(DisplayName = "Routine"),
    Important UMETA(DisplayName = "Important"),
    Urgent    UMETA(DisplayName = "Urgent"),
    Critical  UMETA(DisplayName = "Critical"),
};

USTRUCT(BlueprintType)
struct DODA_API FOperationRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOperationId OperationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCaseId CaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOperationType OperationType = EOperationType::Investigation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOperationStatus Status = EOperationStatus::Proposed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOperationRisk Risk = EOperationRisk::Low;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOperationPriority Priority = EOperationPriority::Routine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId LocationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndTime = 0.f;

    bool IsValid() const { return OperationId.IsValid(); }
};

USTRUCT(BlueprintType)
struct DODA_API FOperationPhase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOpPhaseId PhaseId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOperationId OperationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PhaseName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOperationStatus PhaseStatus = EOperationStatus::Proposed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StartTime = 0.f;

    bool IsValid() const { return PhaseId.IsValid(); }
};

USTRUCT(BlueprintType)
struct DODA_API FOperationAssignment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOperationId OperationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId AgentId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERoleClass AgentRole = ERoleClass::FieldAgent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AssignedTime = 0.f;
};

USTRUCT(BlueprintType)
struct DODA_API FOperationOutcome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOperationId OperationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAssignmentOutcome Result = EAssignmentOutcome::Pending;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEvidenceId> EvidenceGained;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPawnId> SuspectsIdentified;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAlertTriggered = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Notes;
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnOperationStatusChanged, FOperationId, EOperationStatus);

UCLASS()
class DODA_API UOperationSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    FOperationId CreateOperation(FCaseId CaseId, EOperationType Type,
        EOperationPriority Priority, FRoomId LocationId);
    FOperationRecord* GetOperationMutable(FOperationId Id);
    const FOperationRecord* GetOperation(FOperationId Id) const;
    void SetOperationStatus(FOperationId Id, EOperationStatus NewStatus);

    FOpPhaseId AddPhase(FOperationId OpId, const FString& PhaseName);
    TArray<FOperationPhase> GetPhasesForOperation(FOperationId OpId) const;

    void AssignAgent(FOperationId OpId, FPawnId AgentId, ERoleClass Role, float Time);
    TArray<FOperationAssignment> GetAssignmentsForOperation(FOperationId OpId) const;
    TArray<FOperationAssignment> GetAssignmentsForAgent(FPawnId AgentId) const;

    void RecordOutcome(FOperationId OpId, const FOperationOutcome& Outcome);
    const FOperationOutcome* GetOutcome(FOperationId OpId) const;

    TArray<FOperationRecord> GetOperationsForCase(FCaseId CaseId) const;
    TArray<FOperationRecord> GetActiveOperations() const;
    TArray<FOperationRecord> GetOperationsByType(EOperationType Type) const;

    const TMap<FOperationId, FOperationRecord>& GetAllOperations() const { return Operations; }

    FOnOperationStatusChanged OnOperationStatusChanged;

private:
    TMap<FOperationId, FOperationRecord>  Operations;
    TMap<FOpPhaseId, FOperationPhase>   Phases;
    TArray<FOperationAssignment>          Assignments;
    TMap<FOperationId, FOperationOutcome> Outcomes;

    int32 NextOperationId = 1;
    int32 NextPhaseId = 1;

    FOperationId AllocateOperationId();
    FOpPhaseId   AllocatePhaseId();
};