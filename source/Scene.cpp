//-----------------------------------------------------------------
// Includes
//-----------------------------------------------------------------
#include "pch.h"
#include "Scene.h"
#include "Mesh.h"
#include "Material.h"

using namespace dae;


//-----------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------
Scene::Scene()
{
}


//-----------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------
Scene::~Scene()
{
}


//-----------------------------------------------------------------
// Public Member Functions
//-----------------------------------------------------------------
void Scene::Update(const Timer* pTimer)
{
	//Update camera first since we need to retrieve data from it
	m_Camera.Update(pTimer);

	//Calculate the WorldViewProjection matrix
	Matrix viewProj = m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix();

	//Update Mesh
	for (Mesh*& pMesh : m_pMeshes)
	{
		viewProj = pMesh->GetWorldMatrix() * viewProj;
		pMesh->GetMaterial()->SetMatrix(viewProj);
	}
}

void Scene::Render(ID3D11DeviceContext* pDeviceContext) const
{
	for (Mesh* pMesh : m_pMeshes)
	{
		pMesh->Render(pDeviceContext);
	}
}

void Scene::AddMesh(Mesh* pMesh)
{
	m_pMeshes.emplace_back(pMesh);
}


//-----------------------------------------------------------------
// Private Member Functions
//-----------------------------------------------------------------
