#include <Game/Blocks/WorldGen.hpp>

#include <random>

#include <iostream>

// Maps the linear value from 0 to 1 to a more smooth and curvy line.
static float smoothstep(const float& t) {
	return t * t * (3 - 2 * t); 
}

WorldGen::WorldGen(unsigned int seed)
	: m_noise(0)
{
}

void WorldGen::updateTools()
{
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


// Maybe later optimize the iteration access of Chunks so I can generate a singe noise value for each y.
// Iteration order x z y or z x y
void WorldGen::generateChunk(Chunk& chunk, const Vec3I& chunkPos)
{
	for (int32_t z = 0; z < Chunk::SIZE; z++)
	{
		for (int32_t x = 0; x < Chunk::SIZE; x++)
		{
			const Vec3 heightPoint = Vec3(x + chunkPos.x * Chunk::SIZE, 0.5, z + chunkPos.z * Chunk::SIZE) * heightMapScale;
			// This value is the same for every block on this y value !!!!!!!!!!!!!!!!!!!!!!!!
			float heightValue = 0.0f;
			float lacunarity = 1.0f;
			float persistence = 1.0f;
			for (int i = 1; i <= heightMapOctaves; i++)
			{
				heightValue += m_noise.at(heightPoint * lacunarity) * persistence;
				lacunarity *= heightMapOctaveLacunarity;
				persistence *= heightMapOctavePersitence;
			}

			for (int32_t y = 0; y < Chunk::SIZE; y++)
			{
				const Vec3I posInChunk(x, y, z);
				const Vec3 pos(posInChunk + chunkPos * Chunk::SIZE);


				float noiseValue = 0.0f;
				float lacunarity2 = 1.0f;
				float persistence2 = 1.0f;
				for (int i = 1; i <= noiseOctaves; i++)
				{
					lacunarity2 *= noiseOctaveLacunarity;
					persistence2 *= noiseOctavePersitence;
					noiseValue += m_noise.at(pos * noiseScale * lacunarity2) * persistence2;
				}

				heightValue += noiseValue * noiseValueScale;
				heightValue /= 2.0f;
				int32_t height = heightValue * heightMapAmplitude;

				if (pos.y <= height)
				{
					chunk.set(posInChunk, BlockType::Cobblestone);
				}
				else
				{
					chunk.set(posInChunk, BlockType::Air);
				}
			}
		}
	}

//	int iterations = 6;
//	float scale = 3.0f;
//	for (int32_t z = 0; z < Chunk::SIZE; z++)
//	{
//		for (int32_t x = 0; x < Chunk::SIZE; x++)
//		{
//			Vec3 heightPoint = Vec3(x + chunkPos.x * Chunk::SIZE, 0.5, z + chunkPos.z * Chunk::SIZE) / scale;
//			float heightNoise = m_noise.at(heightPoint / 10.0f);
//			for (int i = 2; i < iterations; i++)
//			{
//				heightNoise += m_noise.at(heightPoint / (i * 5)) * i / iterations;
//			}
//
//			for (int32_t y = 0; y < Chunk::SIZE; y++)
//			{
//				Vec3I posInChunk(x, y, z);
//				Vec3 pos(chunkPos * Chunk::SIZE + posInChunk);
//				pos /= scale;
//
//				float noise = m_noise.at(pos / 10.0f);
//				for (int i = 2; i < iterations; i++)
//				{
//					noise -= m_noise.at(pos / i) * i / iterations;
//				}
//
//				heightNoise += noise / 4.0f;
//				heightNoise *= m_noise.at((heightPoint) / 50.0f);
//				int32_t height = heightNoise * 20;
//
//
///*				if (int32_t(pos.y) == int32_t(heightNoise * 5))
//				{
//					chunk.set(posInChunk, BlockType::Grass);
//				}
//				else */if (pos.y < heightNoise * 2)
//				{
//					chunk.set(posInChunk, BlockType::Dirt);
//				}
//				else
//				{
//					chunk.set(posInChunk, BlockType::Air);
//				}
//
//				if (noise > 0.0f)
//					chunk.set(posInChunk, BlockType::Dirt);
//				else
//					chunk.set(posInChunk, BlockType::Air);
//			}
//		}
//	}


	//for (int32_t z = 0; z < Chunk::SIZE; z++)
	//{
	//	for (int32_t x = 0; x < Chunk::SIZE; x++)
	//	{
	//		float heightValue = m_noise.accumulatedOctaveAt(Vec3(x + chunkPos.x * Chunk::SIZE, 0.5f, z + chunkPos.z * Chunk::SIZE));
	//		/*float heightMultiplier = m_noise.accumulatedOctaveAt(Vec3((x + chunkPos.x * Chunk::SIZE) * 0.2, 0.5f, (z + chunkPos.z * Chunk::SIZE) * 0.2));*/
	//		float heightMultiplier = m_noise.at(Vec3((x + chunkPos.x * Chunk::SIZE) * 0.01, 0.5f, (z + chunkPos.z * Chunk::SIZE) * 0.01));

	//		for (int32_t y = 0; y < Chunk::SIZE; y++)
	//		{
	//			Vec3I posInChunk(x, y, z);
	//			Vec3I pos = chunkPos * Chunk::SIZE + posInChunk;


	//			//value = (value + 1) / 2.0f;
	//		
	//			//int v = static_cast<int>(value * 255);

	//			//int height = noise * 255;

	//			int32_t height = heightValue * 100 * ((heightMultiplier + 1) / 2.0f);

	//			if (pos.y <= height)
	//			{
	//				float caveValue = m_noise.accumulatedOctaveAt(Vec3(pos) * 4.0f);
	//				if (caveValue < 0.2)
	//				{
	//						if (pos.y == height)
	//							chunk.set(posInChunk, BlockType::Grass);
	//						else
	//							chunk.set(posInChunk, BlockType::Dirt);
	//				}
	//					
	//				else
	//					chunk.set(posInChunk, BlockType::Air);
	//			}
	//			else
	//				chunk.set(posInChunk, BlockType::Air);
	//		}
	//	}
	//}
}