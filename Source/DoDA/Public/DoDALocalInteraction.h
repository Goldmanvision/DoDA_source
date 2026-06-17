// Source/DoDA/Public/DoDALocalInteraction.h
// Batch 09 -- Local interaction system.
// Component on the player pawn. Bridges 3D field layer to sim layer via ID lookups.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DoDATypes.h"
#include "DoDALocalInteraction.generated.h"

// -----------------------------------------------------------------
// ENUMS
// -----------------------------------------------------------------

UENUM(BlueprintType)
enum class ELocalInteractionType : uint8
{
    Examine     UMETA(DisplayName = "Examine"),
    Collect     UMETA(DisplayName = "Collect"),
    Speak       UMETA(DisplayName = "Speak"),
    Restrain    UMETA(DisplayName = "Restrain"),
    Breach      UMETA(DisplayName = "Breach"),
    Secure      UMETA(DisplayName = "Secure"),
    Deploy      UMETA(DisplayName = "Deploy"),
    Document    UMETA(DisplayName = "Document"),
    Neutralize  UMETA(DisplayName = "Neutralize"),
};

UENUM(BlueprintType)
enum class ELocalTargetType : uint8
{
    Evidence    UMETA(DisplayName = "Evidence"),
    Suspect     UMETA(DisplayName = "Suspect"),
    Civilian    UMETA(DisplayName = "Civilian"),
    Agent       UMETA(DisplayName = "Agent"),
    Door        UMETA(DisplayName = "Door"),
    Container   UMETA(DisplayName = "Container"),
    Anomaly     UMETA(DisplayName = "Anomaly"),
    Device      UMETA(DisplayName = "Device"),
};

UENUM(BlueprintType)
enum class ELocalControlState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Scanning    UMETA(DisplayName = "Scanning"),
    Interacting UMETA(DisplayName = "Interacting"),
    Restricted  UMETA(DisplayName = "Restricted"),
    Cooldown    UMETA(DisplayName = "Cooldown"),
};

UENUM(BlueprintType)
enum class EInteractionResult : uint8
{
    Pending     UMETA(DisplayName = "Pending"),
    Success     UMETA(DisplayName = "Success"),
    Failure     UMETA(DisplayName = "Failure"),
    Blocked     UMETA(DisplayName = "Blocked"),
    Escalated   UMETA(DisplayName = "Escalated"),
};

// -----------------------------------------------------------------
// STRUCTS
// -----------------------------------------------------------------

USTRUCT(BlueprintType)
struct DODA_API FLocalInteractionRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELocalInteractionType InteractionType = ELocalInteractionType::Examine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ELocalTargetType TargetType = ELocalTargetType::Evidence;

    // Sim-layer ID of the target (cast to int32 from whichever typed ID applies)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetSimId = INDEX_NONE;

    // World-space location of the interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldLocation = FVector::ZeroVector;

    // Operation this interaction belongs to (optional)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOperationId OperationId;

    // Room the interaction occurs in
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId RoomId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp = 0.f;
};

USTRUCT(BlueprintType)
struct DODA_API FLocalInteractionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInteractionResult Result = EInteractionResult::Pending;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FeedbackText;

    // Evidence ID surfaced by this interaction (if any)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EvidenceSimId = INDEX_NONE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTriggeredAlert = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCultResponseTriggered = false;
};

// -----------------------------------------------------------------
// COMPONENT
// -----------------------------------------------------------------

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionCompleted,
    FLocalInteractionRequest, Request,
    FLocalInteractionResult, Result);

UCLASS(ClassGroup = (DoDA), meta = (BlueprintSpawnableComponent))
class DODA_API UDoDALocalInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDoDALocalInteractionComponent();

    virtual void BeginPlay() override;

    // Called by player input or TAPLINE command
    UFUNCTION(BlueprintCallable, Category = "DoDA|Interaction")
    FLocalInteractionResult RequestInteraction(const FLocalInteractionRequest& Request);

    UFUNCTION(BlueprintPure, Category = "DoDA|Interaction")
    ELocalControlState GetControlState() const { return ControlState; }

    UFUNCTION(BlueprintCallable, Category = "DoDA|Interaction")
    void SetControlState(ELocalControlState NewState);

    UFUNCTION(BlueprintPure, Category = "DoDA|Interaction")
    TArray<FLocalInteractionRequest> GetInteractionHistory() const { return History; }

    UPROPERTY(BlueprintAssignable, Category = "DoDA|Interaction")
    FOnInteractionCompleted OnInteractionCompleted;

    // Cooldown between interactions in seconds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DoDA|Interaction")
    float InteractionCooldown = 0.5f;

private:
    UPROPERTY()
    ELocalControlState ControlState = ELocalControlState::Idle;

    UPROPERTY()
    TArray<FLocalInteractionRequest> History;

    float LastInteractionTime = 0.f;

    FLocalInteractionResult ProcessExamine(const FLocalInteractionRequest& Request);
    FLocalInteractionResult ProcessCollect(const FLocalInteractionRequest& Request);
    FLocalInteractionResult ProcessSpeak(const FLocalInteractionRequest& Request);
    FLocalInteractionResult ProcessBreach(const FLocalInteractionRequest& Request);
    FLocalInteractionResult ProcessDefault(const FLocalInteractionRequest& Request);
};