//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Mesh.h"
#include "Material.h"

using namespace dae;


//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
Mesh::Mesh(ID3D11Device* pDevice, Material* pMaterial, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	: m_pMaterial(pMaterial)
{
	//Create Vertex Buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices.data();

	HRESULT result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result))
		return;


	//Create Index Buffer
	m_NumIndices = static_cast<uint32_t>(indices.size());
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	initData.pSysMem = indices.data();

	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result))
		return;
}


//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
Mesh::~Mesh()
{
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}


//-----------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------
void Mesh::Render(ID3D11DeviceContext* pDeviceContext) const
{
	//1. Set Primitive Topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//2. Set Input Layout
	pDeviceContext->IASetInputLayout(m_pMaterial->GetInputLayout());

	//3. Set Vertex Buffer
	constexpr UINT stride = sizeof(Vertex);
	constexpr UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);


	//4. Set Index Buffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//5. Draw
	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pMaterial->GetTechnique()->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pMaterial->GetTechnique()->GetPassByIndex(p)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

void Mesh::Translate(const Vector3& translation)
{
	m_Position += translation;
}

void Mesh::Rotate(const Vector3& rotation)
{
	m_Rotation += rotation;
}

void Mesh::SetPosition(float x, float y, float z)
{
	m_Position = Vector3{ x, y, z };
}

void Mesh::SetRotation(float pitch, float yaw, float roll)
{
	m_Rotation = Vector3{ pitch, yaw, roll };
}

void Mesh::SetScale(const Vector3& scale)
{
	m_Scale = scale;
}


//-----------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------

