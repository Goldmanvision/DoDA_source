#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DoDATypes.h"
#include "DoDADebugHUD.generated.h"

UCLASS()
class DODA_API ADoDADebugHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;

private:
    void DrawSimTime();
    void DrawPawnVitals();
    void DrawCultState();
    void DrawCaseState();


    // Layout helpers
    float LineHeight = 18.f;
    float ColA       = 20.f;
    float ColB       = 300.f;
    float ColC       = 580.f;
    float YCursor    = 20.f;

    void DrawLine(const FString& Text, float X, FLinearColor Color = FLinearColor::White);
    void ResetCursor() { YCursor = 20.f; }

    void OnTaskCompleted(FTaskId TaskId, FCaseId CaseId);
};