#include <Game/Blocks/ChunkSystem.hpp>
#include <Utils/Opt.hpp>

class BlockSystem
{
public:
	BlockSystem();

	void update();

	Opt<Block> tryGet(Vec3I blockPos) const;
	void trySet(Vec3I blockPos, BlockType type);

public:
	ChunkSystem chunkSystem;

	BlockData blockData;
};