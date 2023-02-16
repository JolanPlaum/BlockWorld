#pragma once

namespace dae
{
	struct Vertex
	{
		Vector3 position{};
		Vector2 uv{};
		Vector3 normal{};
	};

	struct Coord
	{
		int x{};
		int z{};

		bool operator==(const Coord& c) const
		{
			return x == c.x && z == c.z;
		}
	};

	enum class BlockType : BYTE
	{
		air,

		water,
		dirt,
		grass,
		sand,
		snow,
		stone,
	};

	enum class FaceType : BYTE
	{
		front, back,
		bottom, top,
		left, right,
	};
}