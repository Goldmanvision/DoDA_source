#include "DoDAMeta.h"
#include "Engine/Engine.h"

void UDoDAMetaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    MetaState.CurrentQuarter = 1;
    MetaState.PublicTrust = 50.f;
    MetaState.OversightLevel = 1.f;
    MetaState.MediaHeat = 0.f;
    MetaState.BudgetAvailable = 0;
    MetaState.OpsBudget = 0;
    MetaState.ResearchBudget = 0;
    MetaState.PRBudget = 0;
    RecalculateBudget();
}

void UDoDAMetaSubsystem::RecalculateBudget()
{
    MetaState.PublicTrust = FMath::Clamp(MetaState.PublicTrust, 0.f, 100.f);
    MetaState.OversightLevel = FMath::Clamp(MetaState.OversightLevel, 0.f, 3.f);
    MetaState.MediaHeat = FMath::Clamp(MetaState.MediaHeat, 0.f, 100.f);

    int32 BaseBudget = 100;
    float TrustBonus = (MetaState.PublicTrust - 50.f) * 0.5f;
    float OversightPenalty = MetaState.OversightLevel * 10.f;
    float NewBudget = BaseBudget + TrustBonus - OversightPenalty;

    MetaState.BudgetAvailable = FMath::Max(30, FMath::RoundToInt(NewBudget));

    MetaState.OpsBudget = MetaState.BudgetAvailable / 3;
    MetaState.ResearchBudget = MetaState.BudgetAvailable / 3;
    MetaState.PRBudget = MetaState.BudgetAvailable - MetaState.OpsBudget - MetaState.ResearchBudget;
}

void UDoDAMetaSubsystem::StartNewQuarter()
{
    MetaState.CurrentQuarter++;
    MetaState.MediaHeat = FMath::Max(0.f, MetaState.MediaHeat - 10.f);
    RecalculateBudget();

    UE_LOG(LogTemp, Log, TEXT("DoDA|Meta -- Quarter %d started. Budget: %d"),
        MetaState.CurrentQuarter, MetaState.BudgetAvailable);
}

void UDoDAMetaSubsystem::AllocateBudget(int32 OpsAmount, int32 ResearchAmount, int32 PRAmount)
{
    OpsAmount = FMath::Max(0, OpsAmount);
    ResearchAmount = FMath::Max(0, ResearchAmount);
    PRAmount = FMath::Max(0, PRAmount);

    int32 Total = OpsAmount + ResearchAmount + PRAmount;
    if (Total <= 0)
    {
        return;
    }

    int32 Budget = FMath::Max(1, MetaState.BudgetAvailable);
    if (Total != Budget)
    {
        float Scale = (float)Budget / (float)Total;
        OpsAmount = FMath::RoundToInt(OpsAmount * Scale);
        ResearchAmount = FMath::RoundToInt(ResearchAmount * Scale);
        PRAmount = Budget - OpsAmount - ResearchAmount;
    }

    MetaState.OpsBudget = FMath::Clamp(OpsAmount, 0, MetaState.BudgetAvailable);
    MetaState.ResearchBudget = FMath::Clamp(ResearchAmount, 0, MetaState.BudgetAvailable - MetaState.OpsBudget);
    MetaState.PRBudget = FMath::Clamp(PRAmount, 0, MetaState.BudgetAvailable - MetaState.OpsBudget - MetaState.ResearchBudget);

    int32 AssignedTotal = MetaState.OpsBudget + MetaState.ResearchBudget + MetaState.PRBudget;
    if (AssignedTotal != MetaState.BudgetAvailable)
    {
        MetaState.PRBudget += MetaState.BudgetAvailable - AssignedTotal;
    }
}

void UDoDAMetaSubsystem::ApplyQuarterlyOutcomes()
{
    MetaState.PublicTrust = FMath::Clamp(MetaState.PublicTrust, 0.f, 100.f);
    MetaState.OversightLevel = FMath::Clamp(MetaState.OversightLevel, 0.f, 3.f);
    MetaState.MediaHeat = FMath::Clamp(MetaState.MediaHeat, 0.f, 100.f);

    float PRNorm = (float)MetaState.PRBudget / (float)FMath::Max(1, MetaState.BudgetAvailable);
    float TrustDelta = (PRNorm - 0.33f) * 30.f;
    MetaState.PublicTrust = FMath::Clamp(MetaState.PublicTrust + TrustDelta, 0.f, 100.f);

    float HeatDelta = 5.f - PRNorm * 10.f;
    MetaState.MediaHeat = FMath::Clamp(MetaState.MediaHeat + HeatDelta, 0.f, 100.f);

    MetaState.OversightLevel = FMath::Clamp(
        MetaState.OversightLevel + (MetaState.MediaHeat > 60.f ? 1.f : -0.2f),
        0.f, 3.f
    );

    RecalculateBudget();

    UE_LOG(LogTemp, Log, TEXT("DoDA|Meta -- Quarter %d end. Trust: %.1f, Heat: %.1f, Oversight: %.1f"),
        MetaState.CurrentQuarter, MetaState.PublicTrust, MetaState.MediaHeat, MetaState.OversightLevel);
}