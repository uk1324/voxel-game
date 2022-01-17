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

	ImGui::SliderFloat("hh", &hh, 1.0f, 20.0f);

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

				float hv2 = heightValue;
				float hv3 = heightValue;
				float hv = heightValue;

				float noiseValue = 0.0f;
				float lacunarity2 = 1.0f;
				float persistence2 = 1.0f;
				for (int i = 1; i <= noiseOctaves; i++)
				{
					lacunarity2 *= noiseOctaveLacunarity;
					persistence2 *= noiseOctavePersitence;
					noiseValue += m_noise.at(pos * noiseScale * lacunarity2) * persistence2;
				}

				hv += noiseValue * noiseValueScale;
				//hv /= 2.0f;
				const int32_t height = hv * heightMapAmplitude;

				if (pos.y <= height)
				{
					float noiseValue = 0.0f;
					float lacunarity2 = 1.0f;
					float persistence2 = 1.0f;
					for (int i = 1; i <= noiseOctaves; i++)
					{
						lacunarity2 *= noiseOctaveLacunarity;
						persistence2 *= noiseOctavePersitence;
						noiseValue += m_noise.at((pos + Vec3(0, 1, 0)) * noiseScale * lacunarity2) * persistence2;
					}

					hv2 += noiseValue * noiseValueScale;
					//hv /= 2.0f;
					const int32_t height = hv2 * heightMapAmplitude;

					//float mountainNoise = m_noise.accumulatedOctaveAt(heightPoint + Vec3(125, 652, 2));

					if (pos.y >= height)
					{
						chunk.set(posInChunk, BlockType::Grass);

					}
					else
					{
						float noiseValue = 0.0f;
						float lacunarity2 = 1.0f;
						float persistence2 = 1.0f;
						for (int i = 1; i <= noiseOctaves; i++)
						{
							lacunarity2 *= noiseOctaveLacunarity;
							persistence2 *= noiseOctavePersitence;
							noiseValue += m_noise.at((pos + Vec3(0, 4, 0)) * noiseScale * lacunarity2) * persistence2;
						}

						hv3 += noiseValue * noiseValueScale;
						//hv /= 2.0f;
						const int32_t height = hv3 * heightMapAmplitude;
						
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
				//else if (pos.y < 0)
				//{
				//	chunk.set(posInChunk, BlockType::Water);
				//}
				else
				{
					chunk.set(posInChunk, BlockType::Air);
				}
			}
		}
	}
}	