//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "ChunkManager.h"
#include "Mesh.h"
#include <cassert>

using namespace dae;


//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
ChunkManager::ChunkManager()
{
	for (int i{}; i < m_WorldSize; ++i)
	{
		Coord coord{ i % m_WorldWidth, i / m_WorldWidth };
		m_ChunkCoords[i] = coord;
		LoadChunk(m_Chunks[i], coord);
	}

	m_CameraCoord.x = m_WorldWidth / 2;
	m_CameraCoord.z = m_WorldWidth / 2;
}


//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
ChunkManager::~ChunkManager()
{
}


//-----------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------
void ChunkManager::Update(const Vector3& cameraPosition, std::set<int>& initChunkList)
{
	//Caclulate camera position
	Coord coord{ (int)floor(cameraPosition.x / Chunk::width), (int)floor(cameraPosition.z / Chunk::width) };

	//Check if chunks need to be (un)loaded
	if (coord != m_CameraCoord)
	{
		//Re-used variables
		int chunkDistance{ (m_WorldWidth - 1) / 2 };
		Coord minPosOld{ m_CameraCoord.x - chunkDistance, m_CameraCoord.z - chunkDistance };
		Coord maxPosOld{ m_CameraCoord.x + chunkDistance, m_CameraCoord.z + chunkDistance };
		Coord minPosNew{ coord.x - chunkDistance, coord.z - chunkDistance };
		Coord maxPosNew{ coord.x + chunkDistance, coord.z + chunkDistance };


		//Check movement over X axis
		if (coord.x < m_CameraCoord.x)
		{
			//Calculate distance traveled since last update
			int count{ m_CameraCoord.x - coord.x };
			//if (count > m_WorldWidth) count = m_WorldWidth;
			count = 1;
			for (int z{ minPosOld.z }; z <= maxPosOld.z; ++z)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(maxPosOld.x - i, z) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					Coord newCoord{ minPosOld.x - 1, z };
					m_ChunkCoords[index] = newCoord;
					LoadChunk(m_Chunks[index], newCoord);
					//initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				//initChunkList.insert(GetIndex(minPos.x, z));
			}
			--m_CameraCoord.x;
			--minPosOld.x;
			--maxPosOld.x;
		}
		else if (coord.x > m_CameraCoord.x)
		{
			//Calculate distance traveled since last update
			int count{ coord.x - m_CameraCoord.x };
			//if (count > m_WorldWidth) count = m_WorldWidth;
			count = 1;
			for (int z{ minPosOld.z }; z <= maxPosOld.z; ++z)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(minPosOld.x + i, z) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					Coord newCoord{ maxPosOld.x + 1, z };
					m_ChunkCoords[index] = newCoord;
					LoadChunk(m_Chunks[index], newCoord);
					//initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				//initChunkList.insert(GetIndex(maxPos.x, z));
			}
			++m_CameraCoord.x;
			++minPosOld.x;
			++maxPosOld.x;
		}
		//minPosOld.x = minPosNew.x;
		//maxPosOld.x = maxPosNew.x;

		//Check movement over Z axis
		if (coord.z < m_CameraCoord.z)
		{
			//Calculate distance traveled since last update
			int count{ m_CameraCoord.z - coord.z };
			//if (count > m_WorldWidth) count = m_WorldWidth;
			count = 1;
			for (int x{ minPosOld.x }; x <= maxPosOld.x; ++x)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(x, maxPosOld.z - i) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					Coord newCoord{ x, minPosOld.z - 1 };
					m_ChunkCoords[index] = newCoord;
					LoadChunk(m_Chunks[index], newCoord);
					//initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				//initChunkList.insert(GetIndex(x, minPos.z));
			}
			--m_CameraCoord.z;
		}
		else if (coord.z > m_CameraCoord.z)
		{
			//Calculate distance traveled since last update
			int count{ coord.z - m_CameraCoord.z };
			//if (count > m_WorldWidth) count = m_WorldWidth;
			count = 1;
			for (int x{ minPosOld.x }; x <= maxPosOld.x; ++x)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(x, minPosOld.z + i) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					Coord newCoord{ x, maxPosOld.z + 1 };
					m_ChunkCoords[index] = newCoord;
					LoadChunk(m_Chunks[index], newCoord);
					//initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				//initChunkList.insert(GetIndex(x, maxPos.z));
			}
			++m_CameraCoord.z;
		}

		//Update camera coord
		//m_CameraCoord = coord;
	}
}

void ChunkManager::Render(ID3D11DeviceContext* pDeviceContext) const
{
	for (const Chunk& c : m_Chunks)
	{
		c.GetMesh()->Render(pDeviceContext);
	}
}

Chunk* ChunkManager::GetChunk(int x, int z)
{
	int index = GetIndex(x, z);
	if (index == -1) return nullptr;

	return &m_Chunks[index];
}

const Coord& ChunkManager::GetCoord(int index)
{
	assert(0 <= index && index < m_WorldSize);
	return m_ChunkCoords[index];
}

Chunk& ChunkManager::operator[](int index)
{
	assert(0 <= index && index < m_WorldSize);
	return m_Chunks[index];
}


//-----------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------
void ChunkManager::LoadChunk(Chunk& chunk, Coord coord)
{
	int waterHeight{ 30 }, groundHeight{ 3 }, beachHeight{ 32 };

	for (int x{}; x < Chunk::width; ++x)
	{
		for (int z{}; z < Chunk::width; ++z)
		{
			int worldHeight{ GetHeightMap(coord.x * Chunk::width + x, coord.z * Chunk::width + z) };
			BlockType biome{ GetBiomeBlock(coord.x * Chunk::width + x, coord.z * Chunk::width + z) };

			bool hasWater{};

			for (int y{ Chunk::heigth - 1 }; y >= 0; --y)
			{
				//if y is above ground and above water (note: worldheight can be underwater)
				if (y > worldHeight && y > waterHeight)
				{
					chunk.blocks[x][y][z] = BlockType::air;
					continue;
				}

				//if y is at heighest/lowest block that's not air
				if (y == worldHeight)
				{
					//every block under lowest water block is sand
					if (hasWater || y < beachHeight)
					{
						chunk.blocks[x][y][z] = BlockType::sand;
					}
					else
					{
						chunk.blocks[x][y][z] = biome;
					}
				}
				//if y is heigher than the lowest block point, and the block is under waterheight
				else if (y > worldHeight)
				{
					chunk.blocks[x][y][z] = BlockType::water;
					hasWater = true;
				}
				else if (y < beachHeight)
				{
					chunk.blocks[x][y][z] = BlockType::sand;
				}
				else
				{
					chunk.blocks[x][y][z] = (biome == BlockType::grass) ? BlockType::dirt : biome;
				}
			}


			//for (int y{}; y < Chunk::heigth; ++y)
			//{
			//	chunk.blocks[x][y][z] = GetBlockType(x, y, z, (coord.x + coord.z) % 2);
			//}
		}
	}

	chunk.NeedsInit();
}

int ChunkManager::GetHeightMap(int x, int z)
{
	//predefined variables
	int nrOctaves{ 5 };
	int mapZoom{ 10 };

	float worldLevel{};
	float maxValue{};
	for (int i{ 1 }; i <= nrOctaves; ++i)
	{
		maxValue += 1.0f / i;
		worldLevel += Utils::PerlinFunction(float(x) / Chunk::width, float(z) / Chunk::width, i, mapZoom) / i;
	}
	worldLevel /= (maxValue / 2.f);
	worldLevel /= 2.f;
	worldLevel += 0.5f;
	worldLevel *= (Chunk::heigth / 4.f);

	return static_cast<int>(worldLevel);
}

BlockType ChunkManager::GetBlockType(int x, int y, int z, bool grass)
{
	int waterHeight{ 10 }, groundHeight{ 3 };
	int dirtHeight = std::max(x, z) + groundHeight;

	if (y < dirtHeight) return grass ? BlockType::grass : BlockType::dirt;
	if (y < waterHeight) return BlockType::water;
	return BlockType::air;
}

BlockType ChunkManager::GetBiomeBlock(int x, int z)
{
	float temperature{};
	float humidity{};
	int nrOctaves{ 6 };
	int mapZoom{ 190 };

	constexpr float biomeZoom{ 190.0f };
	constexpr float temperatureOffset{ 25.0f };
	constexpr float humidityOffset{ 75.0f };

	float maxValue{};
	for (int i{ 1 }; i <= nrOctaves; ++i)
	{
		maxValue += 1.0f / i;
		temperature += Utils::PerlinFunction(static_cast<float>(x) / Chunk::width, static_cast<float>(z) / Chunk::width, i, biomeZoom, temperatureOffset) / i;
		humidity += Utils::PerlinFunction(static_cast<float>(x) / Chunk::width, static_cast<float>(z) / Chunk::width, i, biomeZoom, humidityOffset) / i;
	}

	temperature /= (maxValue / 2.0f);
	humidity /= (maxValue / 2.0f);

	temperature /= 2.0f;
	humidity /= 2.0f;

	temperature += 0.5f;
	humidity += 0.5f;

	if (humidity > 0.5f)
	{
		return BlockType::grass;
	}
	else
	{
		if (temperature > 0.6f)
		{
			return BlockType::sand;
		}
		else if (temperature < 0.5f)
		{
			return BlockType::snow;
		}
		else
		{
			return BlockType::grass;
		}
	}
}

int ChunkManager::GetIndex(int x, int z) const
{
	auto it = std::find(m_ChunkCoords.begin(), m_ChunkCoords.end(), Coord{ x, z });
	if (it == m_ChunkCoords.end())
	{
		return -1;
	}

	return static_cast<int>(it - m_ChunkCoords.begin());
}

