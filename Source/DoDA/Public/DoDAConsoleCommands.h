#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DoDAPawn.h"
#include "DoDACase.h"
#include "DoDASimOrchestrator.h"
#include "DoDAConsoleCommands.generated.h"

UCLASS()
class DODA_API UDoDAConsoleCommands : public UObject
{
    GENERATED_BODY()
public:
    void RegisterCommands(UWorld* InWorld);

private:
    IConsoleCommand* CmdDumpPawns = nullptr;
    IConsoleCommand* CmdDumpTasks = nullptr;
    IConsoleCommand* CmdDumpSim = nullptr;
    UWorld* World = nullptr;
};