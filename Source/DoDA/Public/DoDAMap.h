// Source/DoDA/Public/DoDAMap.h
// Batch 04 -- Map/Location structs and UMapSubsystem.
// ASCII-only.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DoDATypes.h"
#include "DoDAMap.generated.h"


USTRUCT(BlueprintType)
struct DODA_API FTown
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTownId TownId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Seed = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D WorldOffset = FVector2D::ZeroVector;

    bool IsValid() const { return TownId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FBlock
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBlockId BlockId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FTownId TownId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EZoningType ZoningType = EZoningType::Residential;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D GridCoord = FVector2D::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FBlockId> AdjacentBlocks;

    bool IsValid() const { return BlockId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FLot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLotId LotId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBlockId BlockId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Frontage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Depth = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D LocalOffset = FVector2D::ZeroVector;

    bool IsValid() const { return LotId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FBuilding
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBuildingId BuildingId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLotId LotId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EBuildingType BuildingType = EBuildingType::House;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Address;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D LocalOffset = FVector2D::ZeroVector;

    bool IsValid() const { return BuildingId.IsValid(); }
};


USTRUCT(BlueprintType)
struct DODA_API FRoom
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRoomId RoomId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBuildingId BuildingId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERoomType RoomType = ERoomType::Generic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Tags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FRoomId> ConnectedRooms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D LocalOffset = FVector2D::ZeroVector;

    bool IsValid() const { return RoomId.IsValid(); }
};


UCLASS()
class DODA_API UMapSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    void GenerateTown(int32 Seed);

    const FTown* GetTown(FTownId Id) const;
    const FBlock* GetBlock(FBlockId Id) const;
    const FLot* GetLot(FLotId Id) const;
    const FBuilding* GetBuilding(FBuildingId Id) const;
    const FRoom* GetRoom(FRoomId Id) const;

    TArray<FBuildingId> GetBuildingsInLot(FLotId Id) const;
    TArray<FRoomId> GetRoomsInBuilding(FBuildingId Id) const;

    const TMap<FBuildingId, FBuilding>& GetAllBuildings() const { return Buildings; }
    const TMap<FRoomId, FRoom>& GetAllRooms() const { return Rooms; }

private:
    TMap<FTownId, FTown>         Towns;
    TMap<FBlockId, FBlock>       Blocks;
    TMap<FLotId, FLot>           Lots;
    TMap<FBuildingId, FBuilding> Buildings;
    TMap<FRoomId, FRoom>         Rooms;

    int32 NextTownId = 1;
    int32 NextBlockId = 1;
    int32 NextLotId = 1;
    int32 NextBuildingId = 1;
    int32 NextRoomId = 1;

    FTownId     AllocateTownId();
    FBlockId    AllocateBlockId();
    FLotId      AllocateLotId();
    FBuildingId AllocateBuildingId();
    FRoomId     AllocateRoomId();
};