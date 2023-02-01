#pragma once

namespace dae
{
	struct Vertex
	{
		Vector3 position{};
		Vector2 uv{};
	};

	enum class BlockType : BYTE
	{
		air,

		water,
		dirt,
		sand,
		stone,
	};

	enum class FaceDirection : BYTE
	{
		front, back,
		bottom, top,
		left, right,
	};
}