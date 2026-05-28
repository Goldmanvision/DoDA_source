#include "DoDADebugHUD.h"
#include "DoDASimOrchestrator.h"
#include "DoDAPawn.h"
#include "DoDACult.h"
#include "DoDACase.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"

void ADoDADebugHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;

    ResetCursor();
    DrawSimTime();
    YCursor += LineHeight;
    DrawPawnVitals();
    YCursor += LineHeight;
    DrawCultState();
    YCursor += LineHeight;
    DrawCaseState();
}

void ADoDADebugHUD::BeginPlay()
{
    Super::BeginPlay();

    UCaseSubsystem* CaseSys = GetWorld()->GetSubsystem<UCaseSubsystem>();
    if (CaseSys)
    {
        CaseSys->OnTaskComplete.AddUObject(this, &ADoDADebugHUD::OnTaskCompleted);
    }
}

void ADoDADebugHUD::OnTaskCompleted(FTaskId TaskId, FCaseId CaseId)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("DoDA|HUD -- Task %d Complete (Case %d)"),
                TaskId.Value, CaseId.Value));
    }
}

void ADoDADebugHUD::DrawLine(const FString& Text, float X, FLinearColor Color)
{
    DrawText(Text, Color, X, YCursor);
    YCursor += LineHeight;
}

void ADoDADebugHUD::DrawSimTime()
{
    UDoDASimOrchestrator* Orch = GetWorld()->GetSubsystem<UDoDASimOrchestrator>();
    if (!Orch)
    {
        DrawLine(TEXT("[SIM] Orchestrator not found"), ColA, FLinearColor::Red);
        return;
    }

    DrawLine(FString::Printf(TEXT("[SIM] Day: %d  Time: %.0fs  Scale: paused=%s"),
        Orch->GetSimDay(),
        Orch->GetSimTimeSeconds(),
        Orch->IsSimPaused() ? TEXT("YES") : TEXT("no")),
        ColA, FLinearColor::Yellow);
}

void ADoDADebugHUD::DrawPawnVitals()
{
    UPawnSubsystem* PawnSys = GetWorld()->GetSubsystem<UPawnSubsystem>();
    if (!PawnSys)
    {
        DrawLine(TEXT("[PAWNS] subsystem not found"), ColA, FLinearColor::Red);
        return;
    }

    DrawLine(FString::Printf(TEXT("[PAWNS] count=%d"), PawnSys->Count()), ColA, FLinearColor(0.f, 1.f, 1.f, 1.f));

    for (const TPair<FPawnId, FPawnRecord>& Pair : PawnSys->GetAllPawns())
    {
        const FPawnRecord& P = Pair.Value;
        DrawLine(FString::Printf(
            TEXT("  Pawn %d | HP:%.0f  Stam:%.0f  Stress:%.0f  Sanity:%.0f"),
            P.PawnId.Value,
            P.Vitals.BloodCurrent,
            P.Vitals.StaminaCurrent,
            P.Vitals.StressCurrent,
            P.Vitals.SanityCurrent),
            ColA, FLinearColor::White);
    }
}

void ADoDADebugHUD::DrawCultState()
{
    UCultSubsystem* CultSys = GetWorld()->GetSubsystem<UCultSubsystem>();
    if (!CultSys)
    {
        DrawLine(TEXT("[CULT] subsystem not found"), ColA, FLinearColor::Red);
        return;
    }

    TArray<FCultCell> Cells = CultSys->GetAllCells();
    DrawLine(FString::Printf(TEXT("[CULT] cells=%d"), Cells.Num()), ColA, FLinearColor::Green);

    for (const FCultCell& Cell : Cells)
    {
        FString StateStr;
        switch (Cell.ThreatState)
        {
            case EHazardState::None:      StateStr = TEXT("NONE");      break;
            case EHazardState::Latent:    StateStr = TEXT("LATENT");    break;
            case EHazardState::Active:    StateStr = TEXT("ACTIVE");    break;
            case EHazardState::Critical:  StateStr = TEXT("CRITICAL");  break;
            case EHazardState::Contained: StateStr = TEXT("CONTAINED"); break;
            default:                      StateStr = TEXT("?");         break;
        }

        FLinearColor CellColor = (Cell.ThreatState == EHazardState::Critical)
            ? FLinearColor::Red
            : FLinearColor::White;

        DrawLine(FString::Printf(
            TEXT("  Cell %d [%s] Influence:%.1f  exposed=%s"),
            Cell.CellId.Value,
            *Cell.CellName,
            Cell.Influence,
            Cell.bExposed ? TEXT("YES") : TEXT("no")),
            ColA, CellColor);
    }
}

void ADoDADebugHUD::DrawCaseState()
{
    UCaseSubsystem* CaseSys = GetWorld()->GetSubsystem<UCaseSubsystem>();
    if (!CaseSys)
    {
        DrawLine(TEXT("[CASES] subsystem not found"), ColA, FLinearColor::Red);
        return;
    }

    const TMap<FCaseId, FCase>& AllCases = CaseSys->GetAllCases();
    DrawLine(FString::Printf(TEXT("[CASES] count=%d"), AllCases.Num()), ColA, FLinearColor(0.f, 1.f, 1.f, 1.f));

    for (const TPair<FCaseId, FCase>& Pair : AllCases)
    {
        const FCase& C = Pair.Value;
        TArray<FTask> Tasks = CaseSys->GetActiveTasksForCase(C.CaseId);
        DrawLine(FString::Printf(
            TEXT("  Case %d [%s] tasks=%d"),
            C.CaseId.Value,
            *C.Title,
            Tasks.Num()),
            ColA, FLinearColor::White);
    }
}