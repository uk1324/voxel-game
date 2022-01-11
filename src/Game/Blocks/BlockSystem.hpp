#include <Game/Blocks/ChunkSystem.hpp>
#include <Utils/Opt.hpp>

class BlockSystem
{
public:
	struct RaycastHit
	{
	public:
		Vec3I blockPos;
		// nullopt if the blockPos == startPos
		Opt<Vec3I> entryNormal;
	};

public:
	BlockSystem();

	Opt<Block> tryGet(const Vec3I& blockPos) const;
	void trySet(const Vec3I& blockPos, Block block);

	bool isSolid(Vec3I& blockPos);

	Opt<RaycastHit> raycast(const Vec3& start, const Vec3& end);

public:
	ChunkSystem chunkSystem;
	BlockData blockData;
};