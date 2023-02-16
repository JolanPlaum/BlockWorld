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
		void Render(ID3D11DeviceContext* pDeviceContext) const;

		static const int m_WorldWidth{ 127 };
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

		void LoadChunk(Chunk& chunk, Coord coord);
		int GetHeightMap(int x, int z);
		BlockType GetBlockType(int x, int y, int z, bool grass);

		//int GetIndex(const Chunk& c) const;
		BlockType GetBiomeBlock(int x, int z);
		int GetIndex(int x, int z) const;
	};
}

