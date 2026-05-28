#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDASimOrchestrator.generated.h"

UCLASS()
class DODA_API UDoDASimOrchestrator : public UTickableWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    // Sim time controls
    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    void SetTimeScale(float Scale) { TimeScale = FMath::Clamp(Scale, 0.f, 10.f); }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    void PauseSim() { bPaused = true; }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    void ResumeSim() { bPaused = false; }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    float GetSimTimeSeconds() const { return SimTimeSeconds; }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    int32 GetSimDay() const { return FMath::FloorToInt(SimTimeSeconds / SecondsPerDay); }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Sim")
    bool IsSimPaused() const { return bPaused; }

private:
    float SimTimeSeconds   = 0.f;
    float TimeScale        = 1.f;
    bool  bPaused          = false;

    // Scheduler fires every SchedulerInterval sim-seconds
    float SchedulerIntervalMin = 2700.f;   // 45 sim-minutes
    float SchedulerIntervalMax = 4500.f;   // 75 sim-minutes
    float SchedulerInterval = 7.f;   // will be randomized on first run
    float TimeSinceLastScheduler = 0.f;

    // Cult/paranormal step interval
    float CultStepInterval      = 600.f;     // 10 sim-minutes
    float TimeSinceLastCultStep = 0.f;

    static constexpr float SecondsPerDay = 86400.f;

    void TickVitals(float DeltaSimTime);
    void TickCult(float DeltaSimTime);
    void TickScheduler(float DeltaSimTime);
    void TickTasks(float DeltaSimTime);
};