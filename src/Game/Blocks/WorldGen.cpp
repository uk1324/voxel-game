#include <Game/Blocks/WorldGen.hpp>
#include <Math/Other.hpp>

#include <random>

#include <iostream>

// Maps the linear value from 0 to 1 to a more smooth and curvy line.
static float smoothstep(const float& t) {
	return t * t * (3 - 2 * t); 
}

WorldGen::WorldGen(unsigned int seed)
	: m_noise(0)
{
	std::cout << "limit: " << overhangNoiseLimit() << '\n';
}

void WorldGen::updateTools()
{
	return;
	ImGui::Begin("World generation");

	ImGui::Text("Height map");
	ImGui::SliderFloat("heightMapScale", &heightMapScale, 0.005f, 0.1f);
	ImGui::SliderFloat("heightMapAmplitude", &heightMapAmplitude, 1.0f, 50.0f);
	ImGui::SliderInt("heightMapOctaves", &heightMapOctaves, 0, 6);
	ImGui::SliderFloat("heightMapOctaveLacunarity", &heightMapOctaveLacunarity, 0.05f, 2.0f);
	ImGui::SliderFloat("heightMapOctavePersitence", &heightMapOctavePersitence, 0.05f, 0.5f);

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Text("Noise");
	ImGui::SliderFloat("noiseScale", &noiseScale, 0.005f, 0.1f);
	ImGui::SliderInt("noiseOctaves", &noiseOctaves, 0, 6);
	ImGui::SliderFloat("noiseOctaveLacunarity", &noiseOctaveLacunarity, 0.05f, 2.0f);
	ImGui::SliderFloat("noiseOctavePersitence", &noiseOctavePersitence, 0.05f, 0.5f);
	ImGui::SliderFloat("noiseValueScale", &noiseValueScale, 0.05f, 5.0f);

	ImGui::End();
}

#include <iostream>
#include <chrono>

// Maybe later optimize the iteration access of Chunks so I can generate a singe noise value for each y.
// Iteration order x z y or z x y
void WorldGen::generateChunk(Chunk& chunk, const Vec3I& chunkPos)
{
	std::vector<Vec3I> treePositions = getTreePositions(chunkPos);

	Structure treeStructure;
	memset(treeStructure.data, 0, sizeof(treeStructure.data));
	treeStructure.width = 5;
	treeStructure.height = 7;
	treeStructure.depth = 5;

	treePositions.erase(
		std::remove_if(treePositions.begin(), treePositions.end(), [chunkPos, treeStructure](const Vec3I& pos) {
			return (pos.x + treeStructure.width < chunkPos.x * Chunk::SIZE)
				|| (pos.x > chunkPos.x * Chunk::SIZE + Chunk::SIZE)
				|| (pos.y + treeStructure.height < chunkPos.y * Chunk::SIZE)
				|| (pos.y > chunkPos.y * Chunk::SIZE + Chunk::SIZE)
				|| (pos.z + treeStructure.depth < chunkPos.z * Chunk::SIZE)
				|| (pos.z > chunkPos.z * Chunk::SIZE + Chunk::SIZE);
		}),
		treePositions.end()
	);

	treeStructure.data[2][0][2] = BlockType::Log;
	treeStructure.data[2][1][2] = BlockType::Log;
	treeStructure.data[2][2][2] = BlockType::Log;
	treeStructure.data[2][3][2] = BlockType::Log;
	treeStructure.data[2][4][2] = BlockType::Log;

	// Tree layer 1
	treeStructure.data[1][3][0] = BlockType::Leaves;
	treeStructure.data[2][3][0] = BlockType::Leaves;
	treeStructure.data[3][3][0] = BlockType::Leaves;

	treeStructure.data[0][3][1] = BlockType::Leaves;
	treeStructure.data[1][3][1] = BlockType::Leaves;
	treeStructure.data[2][3][1] = BlockType::Leaves;
	treeStructure.data[3][3][1] = BlockType::Leaves;
	treeStructure.data[4][3][1] = BlockType::Leaves;

	treeStructure.data[0][3][2] = BlockType::Leaves;
	treeStructure.data[1][3][2] = BlockType::Leaves;
	treeStructure.data[3][3][2] = BlockType::Leaves;
	treeStructure.data[4][3][2] = BlockType::Leaves;

	treeStructure.data[0][3][3] = BlockType::Leaves;
	treeStructure.data[1][3][3] = BlockType::Leaves;
	treeStructure.data[2][3][3] = BlockType::Leaves;
	treeStructure.data[3][3][3] = BlockType::Leaves;
	treeStructure.data[4][3][3] = BlockType::Leaves;

	treeStructure.data[1][3][4] = BlockType::Leaves;
	treeStructure.data[2][3][4] = BlockType::Leaves;
	treeStructure.data[3][3][4] = BlockType::Leaves;

	// Tree layer 2

	treeStructure.data[1][4][0] = BlockType::Leaves;
	treeStructure.data[2][4][0] = BlockType::Leaves;
	treeStructure.data[3][4][0] = BlockType::Leaves;

	treeStructure.data[0][4][1] = BlockType::Leaves;
	treeStructure.data[1][4][1] = BlockType::Leaves;
	treeStructure.data[2][4][1] = BlockType::Leaves;
	treeStructure.data[3][4][1] = BlockType::Leaves;
	treeStructure.data[4][4][1] = BlockType::Leaves;

	treeStructure.data[0][4][2] = BlockType::Leaves;
	treeStructure.data[1][4][2] = BlockType::Leaves;
	treeStructure.data[3][4][2] = BlockType::Leaves;
	treeStructure.data[4][4][2] = BlockType::Leaves;

	treeStructure.data[0][4][3] = BlockType::Leaves;
	treeStructure.data[1][4][3] = BlockType::Leaves;
	treeStructure.data[2][4][3] = BlockType::Leaves;
	treeStructure.data[3][4][3] = BlockType::Leaves;
	treeStructure.data[4][4][3] = BlockType::Leaves;

	treeStructure.data[1][4][4] = BlockType::Leaves;
	treeStructure.data[2][4][4] = BlockType::Leaves;
	treeStructure.data[3][4][4] = BlockType::Leaves;

	treeStructure.data[2][5][2] = BlockType::Leaves;
	treeStructure.data[2][5][2] = BlockType::Leaves;
	treeStructure.data[2][5][1] = BlockType::Leaves;
	treeStructure.data[2][5][3] = BlockType::Leaves;
	treeStructure.data[1][5][2] = BlockType::Leaves;
	treeStructure.data[3][5][2] = BlockType::Leaves;
	treeStructure.data[3][5][3] = BlockType::Leaves;


	treeStructure.data[2][6][2] = BlockType::Leaves;
	treeStructure.data[2][6][2] = BlockType::Leaves;
	treeStructure.data[2][6][1] = BlockType::Leaves;
	treeStructure.data[2][6][3] = BlockType::Leaves;
	treeStructure.data[1][6][2] = BlockType::Leaves;
	treeStructure.data[3][6][2] = BlockType::Leaves;

	float maxHv = 0.0f;

	for (int32_t z = 0; z < Chunk::SIZE; z++)
	{
		for (int32_t x = 0; x < Chunk::SIZE; x++)
		{
			const Vec2 posXz(x + chunkPos.x * Chunk::SIZE, z + chunkPos.z * Chunk::SIZE);
			float heightValue = heightMapNoise(posXz);

			for (int32_t y = 0; y < Chunk::SIZE; y++)
			{
				const Vec3I posInChunk(x, y, z);
				const Vec3 pos(posInChunk + chunkPos * Chunk::SIZE);

				const int32_t height = (heightValue + overhangNoise(pos)) * heightMapAmplitude * 3;

				float hv = (heightValue + overhangNoise(pos));
				if (hv > maxHv)
				{
					maxHv = hv;
				}

				if (pos.y <= height)
				{
					const int32_t height = (heightValue + overhangNoise(pos + Vec3::up)) * heightMapAmplitude * 3;

					if (pos.y >= height)
					{
						chunk.set(posInChunk, BlockType::Grass);
					}
					else
					{
						const int32_t height = (heightValue + overhangNoise(pos + Vec3::up * 4)) * heightMapAmplitude * 3;

						if (pos.y + 4 <= height)
						{
							chunk.set(posInChunk, BlockType::Stone);
						}
						else
						{
							chunk.set(posInChunk, BlockType::Dirt);
						}
					}
				}
				else
				{
					chunk.set(posInChunk, BlockType::Air);
				}

				// I assume linear search is faster than using a set because only a few chunks contain trees.
				// It is also hard to benchmark this code because of how quickly it executes.
				for (const Vec3I& tree : treePositions)
				{
					if (pos.x >= tree.x && pos.x < tree.x + treeStructure.width
						&& pos.y >= tree.y && pos.y < tree.y + treeStructure.height
						&& pos.z >= tree.z && pos.z < tree.z + treeStructure.depth)
					{
						Vec3I p = Vec3I(pos) - tree;
						if (treeStructure.data[p.x][p.y][p.z] != BlockType::Air)
							chunk.set(posInChunk, treeStructure.data[p.x][p.y][p.z]);
						// Not breaking here because multiple trees can overlap.
					}
				}
			}
		}
	}
	//std::cout << "maxHv: " << maxHv << '\n';
}	

float WorldGen::overhangNoise(const Vec3& pos)
{
	return m_noise.accumulatedValue3d(pos * noiseScale, noiseOctaves, noiseOctaveLacunarity, noiseOctavePersitence);
}

float WorldGen::heightMapNoise(const Vec2& posXz)
{
	return m_noise.accumulatedValue2d(posXz * heightMapScale, heightMapOctaves, heightMapOctaveLacunarity, heightMapOctavePersitence);
}

float WorldGen::overhangNoiseLimit()
{
	return noiseOctavePersitence * ((1.0f - pow(noiseOctavePersitence, noiseOctaves)) / (1.0f - noiseOctavePersitence));
}

float WorldGen::heightMapNoiseLimit()
{
	return 0.0f;
}

// This probably is a pretty bad hash function but it doesn't look like it generates any readily apparent patterns.
float WorldGen::hash(const Vec2I& vec)
{
	std::mt19937 gen(vec.x * 1123 + vec.y);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(gen);
}

std::vector<Vec3I> WorldGen::getTreePositions(const Vec3I& chunkPos)
{
	std::vector<Vec3I> treePositions;

	Vec2I start = (Vec2I(chunkPos.x, chunkPos.z) - Vec2I(1)) * Chunk::SIZE;
	Vec2I end = (Vec2I(chunkPos.x, chunkPos.z) + Vec2I(1)) * Chunk::SIZE;

	for (int32_t x = start.x; x < end.x; x++)
	{
		for (int32_t y = start.y; y < end.y; y++)
		{
			if (hash(Vec2I(x, y)) < 0.01)
			{
				int32_t treeY = findGroundY(Vec2(Vec2I(x + 2, y + 2)));
				treePositions.push_back(Vec3I(x, treeY, y));
			}
		}
	}

	return treePositions;
}

// Takes around 8 iterations to find root.
int32_t WorldGen::findGroundY(const Vec2& posXz)
{
	float heightValue = heightMapNoise(posXz);

	float min = -heightMapAmplitude * 3;
	float max = heightMapAmplitude * 3;

	auto at = [heightValue, posXz, this](float y) -> float
	{
		const float height = (heightValue + overhangNoise(Vec3(posXz.x, y, posXz.y))) * heightMapAmplitude * 3;
		return y - height;
	};

	float height = heightValue * heightMapAmplitude * 3;
	float mid = (min + max) / 2.0f;
	while (abs(at(mid)) > 0.25f)
	{
		float atM = at(mid);
		float atA = at(min);

		float signA = signNonZero(atA);
		float signM = signNonZero(atM);

		if (signA == signM)
		{ 
			min = mid;
		}
		else
		{
			max = mid;
		}

		mid = (min + max) / 2.0f;
	}

	return mid;
}