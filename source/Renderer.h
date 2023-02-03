#pragma once

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Scene;
	class Material;
	class Chunk;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&)					= delete;
		Renderer(Renderer&&) noexcept				= delete;
		Renderer& operator=(const Renderer&)		= delete;
		Renderer& operator=(Renderer&&) noexcept	= delete;

		void Update(const Timer* pTimer);
		void Render() const;

	private:
		SDL_Window* m_pWindow{};
		Scene* m_pScene{};
		Material* m_pMaterial{};
		Chunk* m_pChunk{};
		std::set<int> m_InitChunkList{};

		int m_Width{};
		int m_Height{};

		bool m_IsInitialized{ false };

		//Chunk helper functions
		BlockType GetBlockType(int x, int y, int z);
		
		//DIRECTX
		HRESULT InitializeDirectX(IDXGIFactory1*& pDxgiFactory);

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;

		IDXGISwapChain* m_pSwapChain;

		ID3D11Texture2D* m_pDepthStencilBuffer;
		ID3D11DepthStencilView* m_pDepthStencilView;

		ID3D11Texture2D* m_pRenderTargetBuffer;
		ID3D11RenderTargetView* m_pRenderTargetView;

		//SCENES
		Scene* Scene1();
		Scene* Scene2();
		Scene* Scene3();

	};
}
