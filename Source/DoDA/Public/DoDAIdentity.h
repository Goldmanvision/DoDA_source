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
    FString IdentityId;

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

    bool IsValid() const { return !IdentityId.IsEmpty(); }
};


UCLASS()
class DODA_API UIdentitySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void RegisterIdentity(const FIdentityRecord& Record);
    void UnregisterIdentity(const FString& IdentityId);

    const FIdentityRecord* GetById(const FString& IdentityId) const;
    const FIdentityRecord* GetByGuid(const FGuid& WorldGuid) const;

    void SeedTestIdentities(int32 Count);

    const TMap<FString, FIdentityRecord>& GetAll() const { return Records; }
    int32 Count() const { return Records.Num(); }

private:
    TMap<FString, FIdentityRecord> Records;
    TMap<FGuid, FString> GuidIndex;
    int32 NextSeedId = 1;
};