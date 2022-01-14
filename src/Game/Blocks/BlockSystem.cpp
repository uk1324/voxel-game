#include <Game/Blocks/BlockSystem.hpp>

BlockSystem::BlockSystem()
{
}

Opt<Block> BlockSystem::tryGet(const Vec3I& blockPos) const
{
	return chunkSystem.tryGetBlock(blockPos);
}

void BlockSystem::trySet(const Vec3I& blockPos, Block block)
{
	chunkSystem.trySetBlock(blockPos, block);
}

bool BlockSystem::isSolid(Vec3I& blockPos)
{
    const auto optBlock = tryGet(blockPos);
    if (optBlock.has_value())
        return blockData[optBlock.value().type].isSolid;
    return false;
}

// Could optimize this by doing the raycast locally inside a chunks instead of calling
// tryGet each time.
Opt<BlockSystem::RaycastHit> BlockSystem::raycast(const Vec3& start, const Vec3& end)
{
#define SIGN(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))
#define FRAC0(x) (x - floorf(x))
#define FRAC1(x) (1 - x + floorf(x))

    Vec3 ray_start = start;
    Vec3 ray_end = end;

    float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;

    float x1 = ray_start.x, y1 = ray_start.y, z1 = ray_start.z; 
    float x2 = ray_end.x, y2 = ray_end.y, z2 = ray_end.z;

    // Calculates how many d steps does it take to reach the ray end.
    int dx = SIGN(x2 - x1) * Block::SIZE;
    if (dx != 0)
        tDeltaX = fmin(dx / (x2 - x1), std::numeric_limits<float>::infinity());
    else 
        tDeltaX = std::numeric_limits<float>::infinity();

    if (dx > 0)
        tMaxX = tDeltaX * FRAC1(x1);
    else
        tMaxX = tDeltaX * FRAC0(x1);

    int dy = SIGN(y2 - y1);
    if (dy != 0)
        tDeltaY = fmin(dy / (y2 - y1), std::numeric_limits<float>::infinity());
    else
        tDeltaY = std::numeric_limits<float>::infinity();
    
    if (dy > 0)
        tMaxY = tDeltaY * FRAC1(y1);
    else
        tMaxY = tDeltaY * FRAC0(y1);

    int dz = SIGN(z2 - z1);
    if (dz != 0)
        tDeltaZ = fmin(dz / (z2 - z1), std::numeric_limits<float>::infinity());
    else
        tDeltaZ = std::numeric_limits<float>::infinity();

    if (dz > 0)
        tMaxZ = tDeltaZ * FRAC1(z1); 
    else 
        tMaxZ = tDeltaZ * FRAC0(z1);

    Vec3I currentBlockPos(start.applied(floor));
    Vec3I lastBlockPos = currentBlockPos;

    for (;;)
    {
        auto block = tryGet(currentBlockPos);
        if (block.has_value() == false)
        {
            // No hit if block in unloaded chunk.
            return std::nullopt;
        }

        if (block->type != BlockType::Air)
        {
            RaycastHit result;
            result.blockPos = currentBlockPos;
            if (Vec3I(start) == currentBlockPos)
            {
                result.entryNormal = std::nullopt;
                return result;
            }
            result.entryNormal = lastBlockPos - currentBlockPos;
            return result;
        }

        lastBlockPos = currentBlockPos;

        if (tMaxX < tMaxY)
        {
            if (tMaxX < tMaxZ)
            {
                currentBlockPos.x += dx;
                tMaxX += tDeltaX;
            }
            else
            {
                currentBlockPos.z += dz;
                tMaxZ += tDeltaZ;
            }
        }
        else
        {
            if (tMaxY < tMaxZ)
            {
                currentBlockPos.y += dy;
                tMaxY += tDeltaY;
            }
            else
            {
                currentBlockPos.z += dz;
                tMaxZ += tDeltaZ;
            }
        }

        if ((tMaxX > 1) && (tMaxY > 1) && (tMaxZ > 1))
            return std::nullopt;
    }

#undef SIGN
#undef FRAC0
#undef FRAC1
}
