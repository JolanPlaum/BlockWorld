#pragma once
// Includes
#include "Chunk.h"

namespace dae
{
	// Class Forward Declarations
	
	// Class Declaration
	class ChunkManager final
	{
	public:
		// Constructors and Destructor
		static ChunkManager& GetInstance()
		{
			static ChunkManager instance{};
			return instance;
		}
		~ChunkManager();
		
		// Copy and Move semantics
		ChunkManager(const ChunkManager& other)					= delete;
		ChunkManager& operator=(const ChunkManager& other)		= delete;
		ChunkManager(ChunkManager&& other) noexcept				= delete;
		ChunkManager& operator=(ChunkManager&& other) noexcept	= delete;
	
		//---------------------------
		// Public Member Functions
		//---------------------------
		void Update(const Vector3& cameraPosition, std::set<int>& initChunkList);

		static const int m_WorldWidth{ 32 };
		static const int m_WorldSize{ m_WorldWidth * m_WorldWidth };

		Chunk* GetChunk(int x, int z);
		const Coord& GetCoord(int index);

		Chunk& operator[](int index);
	
	
	private:
		// Member variables
		Chunk m_Chunks[m_WorldSize]{};
		std::vector<Coord> m_ChunkCoords{ m_WorldSize };
		Coord m_CameraCoord{};
	
		//---------------------------
		// Private Member Functions
		//---------------------------
		ChunkManager();

		void LoadChunk(Chunk& chunk);
		BlockType GetBlockType(int x, int y, int z);

		//int GetIndex(const Chunk& c) const;
		int GetIndex(int x, int z) const;
	};
}

