// Source/DoDA/Private/DoDAMap.cpp
// Batch 04 -- ASCII-only.

#include "DoDAMap.h"

void UMapSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Towns.Empty();
    Blocks.Empty();
    Lots.Empty();
    Buildings.Empty();
    Rooms.Empty();
    NextTownId = NextBlockId = NextLotId = NextBuildingId = NextRoomId = 1;
}

void UMapSubsystem::Deinitialize()
{
    Towns.Empty();
    Blocks.Empty();
    Lots.Empty();
    Buildings.Empty();
    Rooms.Empty();
    Super::Deinitialize();
}

FTownId UMapSubsystem::AllocateTownId()
{
    FTownId Id;
    Id.Value = NextTownId++;
    return Id;
}

FBlockId UMapSubsystem::AllocateBlockId()
{
    FBlockId Id;
    Id.Value = NextBlockId++;
    return Id;
}

FLotId UMapSubsystem::AllocateLotId()
{
    FLotId Id;
    Id.Value = NextLotId++;
    return Id;
}

FBuildingId UMapSubsystem::AllocateBuildingId()
{
    FBuildingId Id;
    Id.Value = NextBuildingId++;
    return Id;
}

FRoomId UMapSubsystem::AllocateRoomId()
{
    FRoomId Id;
    Id.Value = NextRoomId++;
    return Id;
}

void UMapSubsystem::GenerateTown(int32 Seed)
{
    FMath::SRandInit(Seed);

    Towns.Empty();
    Blocks.Empty();
    Lots.Empty();
    Buildings.Empty();
    Rooms.Empty();

    // Town
    FTown Town;
    Town.TownId = AllocateTownId();
    Town.Name = FString::Printf(TEXT("Town-%d"), Seed);
    Town.Seed = Seed;
    Town.WorldOffset = FVector2D(Seed % 100, Seed % 100) * 1000.f;
    Towns.Add(Town.TownId, Town);

    static const EZoningType ZoneTypes[] = {
        EZoningType::Residential,
        EZoningType::Commercial,
        EZoningType::Industrial,
        EZoningType::Government
    };
    static const EBuildingType BuildingTypes[] = {
        EBuildingType::House,
        EBuildingType::Office,
        EBuildingType::Warehouse,
        EBuildingType::Church,
        EBuildingType::Commercial
    };
    static const ERoomType RoomTypes[] = {
        ERoomType::Generic,
        ERoomType::Office,
        ERoomType::Hallway,
        ERoomType::Basement,
        ERoomType::Bedroom
    };

    TArray<FBlockId> CreatedBlocks;

    // 4 blocks per town
    for (int32 b = 0; b < 4; ++b)
    {
        FBlock Block;
        Block.BlockId = AllocateBlockId();
        Block.TownId = Town.TownId;
        Block.ZoningType = ZoneTypes[b % 4];
        Block.GridCoord = FVector2D(b % 2, b / 2);
        Blocks.Add(Block.BlockId, Block);
        CreatedBlocks.Add(Block.BlockId);

        // 2 lots per block
        for (int32 l = 0; l < 2; ++l)
        {
            FLot Lot;
            Lot.LotId = AllocateLotId();
            Lot.BlockId = Block.BlockId;
            Lot.Frontage = 20.f + (FMath::SRand() * 30.f);
            Lot.Depth = 30.f + (FMath::SRand() * 40.f);
            Lot.LocalOffset = FVector2D(l * 40.f, 0.f);
            Lots.Add(Lot.LotId, Lot);

            // 1 building per lot
            FBuilding Building;
            Building.BuildingId = AllocateBuildingId();
            Building.LotId = Lot.LotId;
            Building.BuildingType = BuildingTypes[(b + l) % 5];
            Building.Address = FString::Printf(TEXT("%d Main St"), 100 + (b * 10) + l);
            Building.LocalOffset = FVector2D(0.f, 0.f);
            Buildings.Add(Building.BuildingId, Building);

            FRoomId PrevRoomId;

            // 3 rooms per building
            for (int32 r = 0; r < 3; ++r)
            {
                FRoom Room;
                Room.RoomId = AllocateRoomId();
                Room.BuildingId = Building.BuildingId;
                Room.RoomType = RoomTypes[r % 5];
                Room.LocalOffset = FVector2D(0.f, r * 12.f);

                if (PrevRoomId.IsValid())
                {
                    Room.ConnectedRooms.Add(PrevRoomId);
                    if (FRoom* PrevRoom = Rooms.Find(PrevRoomId))
                    {
                        PrevRoom->ConnectedRooms.Add(Room.RoomId);
                    }
                }

                Rooms.Add(Room.RoomId, Room);
                PrevRoomId = Room.RoomId;
            }
        }
    }

    // Simple adjacency between blocks in creation order
    for (int32 i = 0; i < CreatedBlocks.Num(); ++i)
    {
        FBlock* Block = Blocks.Find(CreatedBlocks[i]);
        if (!Block) continue;

        if (i > 0)
        {
            Block->AdjacentBlocks.Add(CreatedBlocks[i - 1]);
        }

        if (i + 1 < CreatedBlocks.Num())
        {
            Block->AdjacentBlocks.Add(CreatedBlocks[i + 1]);
        }
    }
}

const FTown* UMapSubsystem::GetTown(FTownId Id) const
{
    return Towns.Find(Id);
}

const FBlock* UMapSubsystem::GetBlock(FBlockId Id) const
{
    return Blocks.Find(Id);
}

const FLot* UMapSubsystem::GetLot(FLotId Id) const
{
    return Lots.Find(Id);
}

const FBuilding* UMapSubsystem::GetBuilding(FBuildingId Id) const
{
    return Buildings.Find(Id);
}

const FRoom* UMapSubsystem::GetRoom(FRoomId Id) const
{
    return Rooms.Find(Id);
}

TArray<FBuildingId> UMapSubsystem::GetBuildingsInLot(FLotId LotId) const
{
    TArray<FBuildingId> Result;
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.LotId == LotId)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FRoomId> UMapSubsystem::GetRoomsInBuilding(FBuildingId BuildingId) const
{
    TArray<FRoomId> Result;
    for (const auto& Pair : Rooms)
    {
        if (Pair.Value.BuildingId == BuildingId)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}