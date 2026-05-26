#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DoDAMeta.generated.h"

USTRUCT(BlueprintType)
struct FMetaState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentQuarter = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 BudgetAvailable = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OpsBudget = 40;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResearchBudget = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PRBudget = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PublicTrust = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OversightLevel = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MediaHeat = 0.f;
};

UCLASS()
class DODA_API UDoDAMetaSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "DoDA|Meta")
    FMetaState GetMetaState() const { return MetaState; }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Meta")
    void StartNewQuarter();

    UFUNCTION(BlueprintCallable, Category = "DoDA|Meta")
    void AllocateBudget(int32 OpsAmount, int32 ResearchAmount, int32 PRAmount);

    UFUNCTION(BlueprintCallable, Category = "DoDA|Meta")
    void ApplyQuarterlyOutcomes();

private:
    UPROPERTY()
    FMetaState MetaState;

    void RecalculateBudget();
};