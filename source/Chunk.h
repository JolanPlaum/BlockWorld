#pragma once
// Includes

namespace dae
{
	// Class Forward Declarations
	class Mesh;
	class Material;
	
	// Class Declaration
	class Chunk final
	{
	public:
		// Constructors and Destructor
		Chunk();
		~Chunk();
		
		// Copy and Move semantics
		Chunk(const Chunk& other)					= delete;
		Chunk& operator=(const Chunk& other)		= delete;
		Chunk(Chunk&& other) noexcept				= delete;
		Chunk& operator=(Chunk&& other) noexcept	= delete;
	
		//---------------------------
		// Public Member Functions
		//---------------------------
		Mesh* Initialize(ID3D11Device* pDevice, Material* pMaterial, Coord coord);
		void Cleanup();

		void NeedsInit() { m_NeedsToInitialize = true; }

		Mesh* GetMesh() const { return m_pMesh; }

		static const int width{ 16 };
		static const int heigth{ 256 };
		BlockType blocks[width][heigth][width]{};
	
	
	private:
		// Member variables
		Mesh* m_pMesh{};
		bool m_NeedsToInitialize{ true };

		//---------------------------
		// Private Member Functions
		//---------------------------
		void AddFaceFront(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
		void AddFaceBack(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
		void AddFaceBottom(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
		void AddFaceTop(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
		void AddFaceLeft(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
		void AddFaceRight(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c);
	
	};
}

