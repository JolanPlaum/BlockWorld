#pragma once

namespace dae
{
	struct Vertex
	{
		Vector3 position{};
		Vector2 uv{};
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
		stone,
	};

	enum class FaceType : BYTE
	{
		front, back,
		bottom, top,
		left, right,
	};
}