// Source/DoDA/Public/DoDAIdentity.h
// Batch 02 -- Identity record struct and Identity subsystem.
// ASCII-only.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDAIdentity.generated.h"


USTRUCT(BlueprintType)
struct DODA_API FIdentityRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIdentityId IdentityId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FullName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Age = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Sex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Address;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Phone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString JobTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Employer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Vehicle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BloodType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid WorldGuid;

    bool IsValid() const { return IdentityId.IsValid(); }
};


UCLASS()
class DODA_API UIdentitySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    FIdentityId RegisterIdentity(FIdentityRecord& Record);
    void UnregisterIdentity(FIdentityId Id);

    const FIdentityRecord* GetById(FIdentityId Id) const;
    const FIdentityRecord* GetByGuid(const FGuid& WorldGuid) const;

    void SeedTestIdentities(int32 Count);

    const TMap<FIdentityId, FIdentityRecord>& GetAll() const { return Records; }
    int32 Count() const { return Records.Num(); }

private:
    TMap<FIdentityId, FIdentityRecord> Records;
    TMap<FGuid, FIdentityId> GuidIndex;
    int32 NextIdentityId = 1;

    FIdentityId AllocateIdentityId();
};