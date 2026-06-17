// Source/DoDA/Private/DoDALocalInteraction.cpp

#include "DoDALocalInteraction.h"
#include "DoDAOperation.h"
#include "DoDACase.h"
#include "Engine/World.h"

UDoDALocalInteractionComponent::UDoDALocalInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDoDALocalInteractionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UDoDALocalInteractionComponent::SetControlState(ELocalControlState NewState)
{
    ControlState = NewState;
}

FLocalInteractionResult UDoDALocalInteractionComponent::RequestInteraction(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;

    // Blocked states
    if (ControlState == ELocalControlState::Restricted ||
        ControlState == ELocalControlState::Interacting)
    {
        Result.Result = EInteractionResult::Blocked;
        Result.FeedbackText = TEXT("Interaction blocked: control state restricted.");
        return Result;
    }

    // Cooldown check
    float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    if ((Now - LastInteractionTime) < InteractionCooldown)
    {
        Result.Result = EInteractionResult::Blocked;
        Result.FeedbackText = TEXT("Interaction on cooldown.");
        return Result;
    }

    ControlState = ELocalControlState::Interacting;
    LastInteractionTime = Now;

    switch (Request.InteractionType)
    {
    case ELocalInteractionType::Examine:   Result = ProcessExamine(Request);  break;
    case ELocalInteractionType::Collect:   Result = ProcessCollect(Request);  break;
    case ELocalInteractionType::Speak:     Result = ProcessSpeak(Request);    break;
    case ELocalInteractionType::Breach:    Result = ProcessBreach(Request);   break;
    default:                               Result = ProcessDefault(Request);  break;
    }

    History.Add(Request);
    ControlState = ELocalControlState::Idle;

    OnInteractionCompleted.Broadcast(Request, Result);
    return Result;
}

FLocalInteractionResult UDoDALocalInteractionComponent::ProcessExamine(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;
    Result.Result = EInteractionResult::Success;
    Result.FeedbackText = TEXT("Examined.");
    return Result;
}

FLocalInteractionResult UDoDALocalInteractionComponent::ProcessCollect(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;

    // Notify case subsystem via operation context if available
    if (GetWorld() && Request.OperationId.IsValid())
    {
        UOperationSubsystem* OpSys = GetWorld()->GetSubsystem<UOperationSubsystem>();
        if (OpSys)
        {
            const FOperationRecord* Op = OpSys->GetOperation(Request.OperationId);
            if (Op && Op->Status == EOperationStatus::Active)
            {
                Result.Result = EInteractionResult::Success;
                Result.FeedbackText = TEXT("Evidence collected and logged to operation.");
                return Result;
            }
        }
    }

    Result.Result = EInteractionResult::Success;
    Result.FeedbackText = TEXT("Collected.");
    return Result;
}

FLocalInteractionResult UDoDALocalInteractionComponent::ProcessSpeak(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;
    Result.Result = EInteractionResult::Success;
    Result.FeedbackText = TEXT("Dialogue initiated.");
    return Result;
}

FLocalInteractionResult UDoDALocalInteractionComponent::ProcessBreach(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;
    Result.Result = EInteractionResult::Success;
    Result.bTriggeredAlert = true;
    Result.FeedbackText = TEXT("Breach executed. Alert state triggered.");
    return Result;
}

FLocalInteractionResult UDoDALocalInteractionComponent::ProcessDefault(
    const FLocalInteractionRequest& Request)
{
    FLocalInteractionResult Result;
    Result.Result = EInteractionResult::Success;
    Result.FeedbackText = TEXT("Action performed.");
    return Result;
}