//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "ChunkManager.h"
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
		LoadChunk(m_Chunks[i]);
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
		Coord minPos{ m_CameraCoord.x - chunkDistance, m_CameraCoord.z - chunkDistance };
		Coord maxPos{ m_CameraCoord.x + chunkDistance, m_CameraCoord.z + chunkDistance };

		//Check movement over X axis
		if (coord.x < m_CameraCoord.x)
		{
			//Calculate distance traveled since last update
			int count{ m_CameraCoord.x - coord.x };
			for (int z{ minPos.z }; z <= maxPos.z; ++z)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(maxPos.x - i, z) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					m_ChunkCoords[index] = { minPos.x - i - 1, z };
					LoadChunk(m_Chunks[index]);
					initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				initChunkList.insert(GetIndex(minPos.x, z));
			}
		}
		else if (coord.x > m_CameraCoord.x)
		{
			//Calculate distance traveled since last update
			int count{ coord.x - m_CameraCoord.x };
			for (int z{ minPos.z }; z <= maxPos.z; ++z)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(minPos.x + i, z) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					m_ChunkCoords[index] = { maxPos.x + i + 1, z };
					LoadChunk(m_Chunks[index]);
					initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				initChunkList.insert(GetIndex(maxPos.x, z));
			}
		}
		minPos.x = coord.x - chunkDistance;
		maxPos.x = coord.x + chunkDistance;

		//Check movement over Z axis
		if (coord.z < m_CameraCoord.z)
		{
			//Calculate distance traveled since last update
			int count{ m_CameraCoord.z - coord.z };
			for (int x{ minPos.x }; x <= maxPos.x; ++x)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(x, maxPos.z - i) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					m_ChunkCoords[index] = { x, minPos.z - i - 1 };
					LoadChunk(m_Chunks[index]);
					initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				initChunkList.insert(GetIndex(x, minPos.z));
			}
		}
		else if (coord.z > m_CameraCoord.z)
		{
			//Calculate distance traveled since last update
			int count{ coord.z - m_CameraCoord.z };
			for (int x{ minPos.x }; x <= maxPos.x; ++x)
			{
				//Loop multiple times in case camera moved more than 1 chunk since last update
				for (int i{}; i < count; ++i)
				{
					//Get index of chunk that needs to be unloaded
					int index{ GetIndex(x, minPos.z + i) };
					if (index == -1) assert(false && "Something went wrong with (un)loading chunks.");

					//Overwrite chunk with new data
					m_ChunkCoords[index] = { x, maxPos.z + i + 1 };
					LoadChunk(m_Chunks[index]);
					initChunkList.insert(index);
				}
				//Reload chunk at the edge of the void since it doesn't have any faces facing the new direction
				initChunkList.insert(GetIndex(x, maxPos.z));
			}
		}

		//Update camera coord
		m_CameraCoord = coord;
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
void ChunkManager::LoadChunk(Chunk& chunk)
{
	for (int x{}; x < Chunk::width; ++x)
	{
		for (int z{}; z < Chunk::width; ++z)
		{
			for (int y{}; y < Chunk::heigth; ++y)
			{
				chunk.blocks[x][y][z] = GetBlockType(x, y, z);
			}
		}
	}
}

BlockType ChunkManager::GetBlockType(int x, int y, int z)
{
	int waterHeight{ 10 }, groundHeight{ 3 };
	int dirtHeight = std::max(x, z) + groundHeight;

	if (y < dirtHeight) return BlockType::dirt;
	if (y < waterHeight) return BlockType::water;
	return BlockType::air;
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

