//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "ChunkManager.h"

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
Chunk* ChunkManager::GetChunk(int x, int z)
{
	auto it = std::find(m_ChunkCoords.begin(), m_ChunkCoords.end(), Coord{x, z});
	if (it == m_ChunkCoords.end()) return nullptr;

	int index = static_cast<int>(it - m_ChunkCoords.begin());
	return &m_Chunks[index];
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

