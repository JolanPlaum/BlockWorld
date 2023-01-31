#pragma once
// Includes

namespace dae
{
	// Class Forward Declarations
	class Texture;
	
	// Class Declaration
	class Material
	{
	public:
		// Constructors and Destructor
		explicit Material(ID3D11Device* pDevice, const std::wstring& assetFile);
		virtual ~Material();
		
		// Copy and Move semantics
		Material(const Material& other)					= delete;
		Material& operator=(const Material& other)		= delete;
		Material(Material&& other) noexcept				= delete;
		Material& operator=(Material&& other) noexcept	= delete;
	
		//---------------------------
		// Public Member Functions
		//---------------------------
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

		//WorldViewProjection Matrix only
		virtual void SetMatrix(Matrix& matrix);
		//Diffuse Texture only
		virtual void SetTexture(Texture* pTexture);

		ID3DX11Effect* GetEffect() const { return m_pEffect; }
		ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; }
		ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }

	
	protected:
		// Member variables
		ID3DX11Effect* m_pEffect{};
		ID3D11InputLayout* m_pInputLayout{};
		ID3DX11EffectTechnique* m_pTechnique{};

		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};
	
		//---------------------------
		// Protected Member Functions
		//---------------------------
	
	};
}
