#pragma once
// Includes

namespace dae
{
	// Forward Declarations
	class Material;

	// Class Declaration
	class Mesh final
	{
	public:
		// Constructors and Destructor
		explicit Mesh(ID3D11Device* pDevice, Material* pMaterial, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		// Copy and Move semantics
		Mesh(const Mesh& other)					= delete;
		Mesh& operator=(const Mesh& other)		= delete;
		Mesh(Mesh&& other) noexcept				= delete;
		Mesh& operator=(Mesh&& other) noexcept	= delete;

		//---------------------------
		// Public Member Functions
		//---------------------------
		void Render(ID3D11DeviceContext* pDeviceContext) const;

		void Translate(const Vector3& translation);
		void Rotate(const Vector3& rotation);

		void SetPosition(int x, int y, int z);
		void SetPosition(float x, float y, float z);
		void SetRotation(float pitch, float yaw, float roll);
		void SetScale(const Vector3& scale);

		Material* GetMaterial() const { return m_pMaterial; }
		Matrix GetWorldMatrix() const { return Matrix::CreateTransform(m_Position, m_Rotation, m_Scale); }


	private:
		// Member variables
		Material* m_pMaterial{};

		Vector3 m_Position{ 0.f, 0.f, 0.f };
		Vector3 m_Rotation{ 0.f, 0.f, 0.f };
		Vector3 m_Scale{ 1.f, 1.f, 1.f };

		uint32_t m_NumIndices{};
		ID3D11Buffer* m_pVertexBuffer{};
		ID3D11Buffer* m_pIndexBuffer{};

		//---------------------------
		// Private Member Functions
		//---------------------------

	};
}
