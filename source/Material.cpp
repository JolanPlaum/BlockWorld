//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Material.h"
#include "Texture.h"
#include <cassert>

using namespace dae;


//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
Material::Material(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	//Load Effect
	m_pEffect = LoadEffect(pDevice, assetFile);

	//Load Techniques
	m_pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (!m_pTechnique->IsValid())
		std::wcout << L"DefaultTechnique not valid\n";

	//Load Matrix
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())
		std::wcout << L"Matrix Variable gWorldViewProj not valid\n";

	//Load Textures
	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid())
		std::wcout << L"Shader Resource gDiffuseMap Variable not valid\n";

	//Create Vertex Layout
	static constexpr uint32_t numElements{ 2 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	vertexDesc[1].SemanticName = "TEXCOORD";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 12;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//Create Input Layout
	D3DX11_PASS_DESC passDesc{};
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	const HRESULT result = pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pInputLayout);

	if (FAILED(result))
		assert(false);
}


//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
Material::~Material()
{
	if (m_pDiffuseMapVariable) m_pDiffuseMapVariable->Release();
	if (m_pMatWorldViewProjVariable) m_pMatWorldViewProjVariable->Release();

	if (m_pInputLayout) m_pInputLayout->Release();
	if (m_pTechnique) m_pTechnique->Release();
	if (m_pEffect) m_pEffect->Release();
}


//-----------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------
ID3DX11Effect* Material::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
{
	HRESULT result;
	ID3D10Blob* pErrorBlob{ nullptr };
	ID3DX11Effect* pEffect;

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	result = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
		&pErrorBlob);

	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

			std::wstringstream ss;
			for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
			{
				ss << pErrors[i];
			}

			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			std::wcout << ss.str() << std::endl;
		}
		else
		{
			std::wstringstream ss;
			ss << "EffectLoader: Failed to CreateEffectFromFile!\nPath: " << assetFile;
			std::wcout << ss.str() << std::endl;
			return nullptr;
		}
	}

	return pEffect;
}

void Material::SetMatrix(Matrix& matrix)
{
#if defined(_DEBUG)
	if (m_pMatWorldViewProjVariable)
		m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
	else
		std::wcout << L"SetMatrix m_pMatWorldViewProjVariable failed\n";
#else
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
#endif
}

void Material::SetTexture(Texture* pTexture)
{
#if defined(_DEBUG)
	if (pTexture)
	{
		if (m_pDiffuseMapVariable)
		{
			const HRESULT result = m_pDiffuseMapVariable->SetResource(pTexture->GetResourceView());
			if (FAILED(result))
				assert(false);
		}
		else
			std::wcout << L"SetDiffuseMap failed\n";
	}
	else
	{
		std::wcout << L"SetDiffuseMap failed: nullptr given\n";
	}
#else
	m_pDiffuseMapVariable->SetResource(pTexture->GetResourceView());
#endif
}


//-----------------------------------------------------------------
// Protected Member Functions
//-----------------------------------------------------------------

