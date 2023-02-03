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
		Mesh* Initialize(ID3D11Device* pDevice, Material* pMaterial);

		static const int width{ 16 };
		static const int heigth{ 256 };
		BlockType blocks[width][heigth][width]{};
	
	
	private:
		// Member variables

		//---------------------------
		// Private Member Functions
		//---------------------------
		void AddFaceFront(std::vector<Vertex>& vector, int x, int y, int z);
		void AddFaceBack(std::vector<Vertex>& vector, int x, int y, int z);
		void AddFaceBottom(std::vector<Vertex>& vector, int x, int y, int z);
		void AddFaceTop(std::vector<Vertex>& vector, int x, int y, int z);
		void AddFaceLeft(std::vector<Vertex>& vector, int x, int y, int z);
		void AddFaceRight(std::vector<Vertex>& vector, int x, int y, int z);
	
	};
}

