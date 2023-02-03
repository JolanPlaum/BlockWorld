#pragma once
namespace dae
{
	namespace Atlas
	{
		enum class Tile : BYTE
		{
			//			  col           row
			//			   x             y

			dirt = (0 << 4 | 15 - 0),
			grass_side = (0 << 4 | 15 - 1),
			grass_top = (0 << 4 | 15 - 2),

			water = (1 << 4 | 15 - 0),
			sand = (1 << 4 | 15 - 1),
		};

		static const float UVSize{ 1.f / 16.f };
		static std::vector<Vector2> GetUVs(BlockType block, FaceType face)
		{
			Tile tile{};

			switch (block)
			{
			case BlockType::water: tile = Tile::water; break;
			case BlockType::dirt: tile = Tile::dirt; break;
			case BlockType::sand: tile = Tile::sand; break;
			case BlockType::grass:
				switch (face)
				{
				case FaceType::front:
				case FaceType::back:
				case FaceType::left:
				case FaceType::right: tile = Tile::grass_side; break;
				case FaceType::top: tile = Tile::grass_top; break;
				case FaceType::bottom: tile = Tile::dirt; break;
				}
			case BlockType::stone: break;
			}

			float col = (BYTE(tile) >> 4) * UVSize;
			float row = (BYTE(tile) & 0xf) * UVSize;

			std::vector<Vector2> uv;
			uv.push_back({ col + UVSize, row });
			uv.push_back({ col + UVSize, row + UVSize });
			uv.push_back({ col, row });
			uv.push_back({ col, row + UVSize });

			return uv;
		}
	}
}
