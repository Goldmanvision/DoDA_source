// Source/DoDA/Public/DoDAScheduler.h
// Batch 05 -- Scheduler subsystem with work-state awareness.
// ASCII-only.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDACase.h"
#include "DoDAPawn.h"
#include "DoDAScheduler.generated.h"


USTRUCT(BlueprintType)
struct DODA_API FAgentView
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId PawnId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERoleClass RoleClass = ERoleClass::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Investigation = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Medicine = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OccultKnowledge = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Surveillance = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Interviewing = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stress = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Stamina = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPawnStatus Status = EPawnStatus::Active;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EWorkState WorkState = EWorkState::Available;
};


USTRUCT(BlueprintType)
struct DODA_API FSchedulerAssignment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTaskId TaskId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPawnId PawnId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost = 0.f;
};


UCLASS()
class DODA_API UDoDASchedulerSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "DoDA|Scheduler")
    void RunScheduler();

    UFUNCTION(BlueprintCallable, Category = "DoDA|Scheduler")
    TArray<FSchedulerAssignment> GetCurrentAssignments() const { return Assignments; }

private:
    UPROPERTY()
    TArray<FSchedulerAssignment> Assignments;

    TArray<FAgentView> BuildAgentViews() const;
    float ComputeCost(const FAgentView& Agent, const FTask& Task) const;
    bool IsAgentAssignable(const FAgentView& Agent) const;

    void OnTaskCompleted(FTaskId TaskId, FCaseId CaseId);
};