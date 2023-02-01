//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Material.h"

using namespace dae;


//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
Chunk::Chunk()
{
}


//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
Chunk::~Chunk()
{
}


//-----------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------
void Chunk::Initialize(ID3D11Device* pDevice, Material* pMaterial)
{
	std::vector<Vertex> vertices{};
	vertices.reserve(width * width * heigth);

	for (int x{}; x < Chunk::width; ++x)
	{
		for (int z{}; z < Chunk::width; ++z)
		{
			for (int y{}; y < Chunk::heigth; ++y)
			{
				if (blocks[x][y][z] == BlockType::air) continue;

				//front
				if (z + 1 < width && blocks[x][y][z + 1] == BlockType::air)
					AddFaceFront(vertices, x, y, z);

				//back
				if (z > 0 && blocks[x][y][z - 1] == BlockType::air)
					AddFaceBack(vertices, x, y, z);

				//top
				if (y + 1 < heigth && blocks[x][y + 1][z] == BlockType::air)
					AddFaceTop(vertices, x, y, z);

				//bottom
				if (y > 0 && blocks[x][y - 1][z] == BlockType::air)
					AddFaceBottom(vertices, x, y, z);

				//right
				if (x + 1 < width && blocks[x + 1][y][z] == BlockType::air)
					AddFaceRight(vertices, x, y, z);

				//left
				if (x > 0 && blocks[x - 1][y][z] == BlockType::air)
					AddFaceLeft(vertices, x, y, z);
			}
		}
	}

	vertices.shrink_to_fit();


	std::vector<uint32_t> indices{};
	indices.reserve(vertices.size() / 4);

	for (unsigned int i{}; i < vertices.size(); i += 4)
	{
		indices.insert(indices.end(), { i + 0, i + 1, i + 2, i + 3, i + 2, i + 1 });
	}


	m_pMesh = new Mesh(pDevice, pMaterial, vertices, indices);
}


//-----------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------
void Chunk::AddFaceFront(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 1.f, y + 1.f, z + 0.f}, {1.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 0.f}, {1.f, 1.f} });
	vector.push_back({ {x + 0.f, y + 1.f, z + 0.f}, {0.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 0.f, z + 0.f}, {0.f, 1.f} });
}

void Chunk::AddFaceBack(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 1.f, y + 1.f, z + 0.f}, {1.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 0.f}, {1.f, 1.f} });
	vector.push_back({ {x + 0.f, y + 1.f, z + 0.f}, {0.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 0.f, z + 0.f}, {0.f, 1.f} });
}

void Chunk::AddFaceBottom(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 0.f, y + 0.f, z + 1.f}, {1.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 0.f, z + 0.f}, {1.f, 1.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 1.f}, {0.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 0.f}, {0.f, 1.f} });
}

void Chunk::AddFaceTop(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 0.f, y + 1.f, z + 0.f}, {1.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 1.f, z + 1.f}, {1.f, 1.f} });
	vector.push_back({ {x + 1.f, y + 1.f, z + 0.f}, {0.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 1.f, z + 1.f}, {0.f, 1.f} });
}

void Chunk::AddFaceLeft(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 0.f, y + 1.f, z + 0.f}, {1.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 0.f, z + 0.f}, {1.f, 1.f} });
	vector.push_back({ {x + 0.f, y + 1.f, z + 1.f}, {0.f, 0.f} });
	vector.push_back({ {x + 0.f, y + 0.f, z + 1.f}, {0.f, 1.f} });
}

void Chunk::AddFaceRight(std::vector<Vertex>& vector, int x, int y, int z)
{
	vector.push_back({ {x + 1.f, y + 1.f, z + 1.f}, {1.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 1.f}, {1.f, 1.f} });
	vector.push_back({ {x + 1.f, y + 1.f, z + 0.f}, {0.f, 0.f} });
	vector.push_back({ {x + 1.f, y + 0.f, z + 0.f}, {0.f, 1.f} });
}
