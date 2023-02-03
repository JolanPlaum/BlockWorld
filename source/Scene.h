#pragma once
// Includes
#include "Camera.h"

namespace dae
{
	// Class Forward Declarations
	class Mesh;
	
	// Class Declaration
	class Scene final
	{
	public:
		// Constructors and Destructor
		explicit Scene();
		explicit Scene(const Camera& camera);
		~Scene();
		
		// Copy and Move semantics
		Scene(const Scene& other)					= delete;
		Scene& operator=(const Scene& other)		= delete;
		Scene(Scene&& other) noexcept				= delete;
		Scene& operator=(Scene&& other) noexcept	= delete;
	
		//---------------------------
		// Public Member Functions
		//---------------------------
		void Update(const Timer* pTimer);
		void Render(ID3D11DeviceContext* pDeviceContext) const;

		void AddMesh(Mesh* pMesh);
		void RemoveMesh(Mesh* pMesh);


	private:
		// Member variables
		Camera m_Camera{};

		std::list<Mesh*> m_pMeshes{};
	
		//---------------------------
		// Private Member Functions
		//---------------------------
	
	};
}
