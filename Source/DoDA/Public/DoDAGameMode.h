#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DoDAGameMode.generated.h"

UCLASS()
class DODA_API ADoDAGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ADoDAGameMode();

    virtual void BeginPlay() override;
};