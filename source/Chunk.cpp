//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Chunk.h"
#include "Mesh.h"
#include "Material.h"
#include "Atlas.h"

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
Mesh* Chunk::Initialize(ID3D11Device* pDevice, Material* pMaterial, Coord coord)
{
	std::vector<Vertex> vertices{};
	vertices.reserve(width * width * heigth);

	coord.x *= width;
	coord.z *= width;

	for (int x{}; x < Chunk::width; ++x)
	{
		for (int z{}; z < Chunk::width; ++z)
		{
			for (int y{}; y < Chunk::heigth; ++y)
			{
				if (blocks[x][y][z] == BlockType::air) continue;

				//front
				if (z + 1 < width && blocks[x][y][z + 1] == BlockType::air)
					AddFaceFront(vertices, x, y, z, coord);

				//back
				if (z > 0 && blocks[x][y][z - 1] == BlockType::air)
					AddFaceBack(vertices, x, y, z, coord);

				//top
				if (y + 1 < heigth && blocks[x][y + 1][z] == BlockType::air)
					AddFaceTop(vertices, x, y, z, coord);

				//bottom
				if (y > 0 && blocks[x][y - 1][z] == BlockType::air)
					AddFaceBottom(vertices, x, y, z, coord);

				//right
				if (x + 1 < width && blocks[x + 1][y][z] == BlockType::air)
					AddFaceRight(vertices, x, y, z, coord);

				//left
				if (x > 0 && blocks[x - 1][y][z] == BlockType::air)
					AddFaceLeft(vertices, x, y, z, coord);
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


	return new Mesh(pDevice, pMaterial, vertices, indices);
}


//-----------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------
void Chunk::AddFaceFront(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::front);
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 0.f}, temp[0] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 0.f}, temp[1] });
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 0.f}, temp[2] });
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 0.f}, temp[3] });
}

void Chunk::AddFaceBack(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::back);
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 0.f}, temp[0] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 0.f}, temp[1] });
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 0.f}, temp[2] });
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 0.f}, temp[3] });
}

void Chunk::AddFaceBottom(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::bottom);
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 1.f}, temp[0] });
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 0.f}, temp[1] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 1.f}, temp[2] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 0.f}, temp[3] });
}

void Chunk::AddFaceTop(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::top);
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 0.f}, temp[0] });
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 1.f}, temp[1] });
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 0.f}, temp[2] });
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 1.f}, temp[3] });
}

void Chunk::AddFaceLeft(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::left);
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 0.f}, temp[0] });
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 0.f}, temp[1] });
	vector.push_back({ {c.x + x + 0.f, y + 1.f, c.z + z + 1.f}, temp[2] });
	vector.push_back({ {c.x + x + 0.f, y + 0.f, c.z + z + 1.f}, temp[3] });
}

void Chunk::AddFaceRight(std::vector<Vertex>& vector, int x, int y, int z, const Coord& c)
{
	std::vector<Vector2> temp = Atlas::GetUVs(blocks[x][y][z], FaceType::right);
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 1.f}, temp[0] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 1.f}, temp[1] });
	vector.push_back({ {c.x + x + 1.f, y + 1.f, c.z + z + 0.f}, temp[2] });
	vector.push_back({ {c.x + x + 1.f, y + 0.f, c.z + z + 0.f}, temp[3] });
}
