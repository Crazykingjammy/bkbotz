#include "RenderInterface.h"
#include "Direct3D/D3dMain.h"

CRenderInterface* CRenderInterface::pInstance = 0;

// Defualt Constructor
CRenderInterface::CRenderInterface()
{
	m_RenderSystem = 0;
	m_ActiveCamera = 0;
	
	//Crete the world and pass in the default model.
	m_World = new CWorld("Models/bigship1.x");

}

void CRenderInterface::DeleteInstance(void)
{
	//If we have instance
	if(pInstance)
	{
		delete pInstance;
		pInstance = 0;
	}
}

CRenderInterface* CRenderInterface::GetInstance()
{
	if(pInstance == 0)
	{
		pInstance = new CRenderInterface;
	}

	return pInstance;
}


void CRenderInterface::AddObject(CBaseObject* object)
{
	// Add on to the vector
	m_ObjectList.push_back(object);

}

void CRenderInterface::AddLabel(CLabel* label)
{
	//Add on to the vector
	m_Objects2D.push_back(label);
}

void CRenderInterface::AddParticle(CBaseParticleEmitter* emitter)
{
	//Add particle emitter to the list.
	m_ParticleList.push_back(emitter);
}

void CRenderInterface::DestroyScene()
{
	//Delete all objects in render
	m_RenderSystem->DestoryScene();


	//Clear the list.
	m_ObjectList.clear();

	//Clear the 2d list.
	m_Objects2D.clear();

	//Hey Destroy the particles.

	//Get number of emmiters to create the number of vertex buffers
	unsigned int numEmitters = (unsigned int)m_ParticleList.size();

	//Call each particles simulate.
	for(unsigned int i = 0 ; i < numEmitters; i++)
	{
		m_ParticleList[i]->Shutdown();
	}

	//Clear the particle list.
	m_ParticleList.clear();


	//Reset the lights.
	for(unsigned int i = 0; i < MAX_LIGHTS; i++)
		Light[i].Reset();

	strcpy(m_World->m_ModelName,"Models/bigship1.x");

	//Set a default Camera GOSH!
	m_ActiveCamera =  &m_DefaultCamera_GOSH;

	//Clean up them particles.
	for(unsigned int i = 0; i < m_ParticleList.size(); i++)
	{
		m_ParticleList[i]->Shutdown();
	}
}

void CRenderInterface::CreateScene()
{
	
	//Create the particles.
	for(unsigned int i = 0; i < m_ParticleList.size(); i++)
	{
		m_ParticleList[i]->CreateParticles();
	}

	m_RenderSystem->CreateScene();
}

void CRenderInterface::Shutdown()
{

	//Destroy the Scene.
	DestroyScene();

	m_RenderSystem->Shutdown();

}

void CRenderInterface::Initialize(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hwnd)
{
	//Init the Render

	//Direct 3d
	if(true)
	{
		m_RenderSystem = new CD3DRender;
	}

	m_RenderSystem->Initalize(hInstance,Width,Height,Fullscreen,hwnd);
}

void CRenderInterface::Update(float timeDelta)
{
	//Hey update the particles.

	//Get number of emmiters to create the number of vertex buffers
	unsigned int numEmitters = (unsigned int)m_ParticleList.size();

	//Call each particles simulate.
	for(unsigned int i = 0 ; i < numEmitters; i++)
	{
		m_ParticleList[i]->Simulate();
	}

	//Call the Render Update
	m_RenderSystem->Update(timeDelta);


}

void CRenderInterface::SetWorld(LPCSTR worldmodel)
{
	//Assign the world.
	strcpy(m_World->m_ModelName,worldmodel);
}

void CRenderInterface::SetActiveCamera(CBaseCamera* camera)
{
	//Set old camera to not active.
	if(m_ActiveCamera)
		m_ActiveCamera->m_Active = false;

	//Set new camera to active.
	camera->m_Active = true;

	//Set current camera.
	m_ActiveCamera = camera;
	
	//If ther rendering system has been created switch the camera in it.
	if(m_RenderSystem)
		m_RenderSystem->SwitchCamera();
}

int CRenderInterface::LoadModel(char* filename)
{
	if(m_RenderSystem)
	{
		//Load that Model.
		return m_RenderSystem->LoadModel(filename);
	}
	else
		return -1;
}

int CRenderInterface::LoadTexture(char *filename)
{
	if(m_RenderSystem)
	{
		//Load that texture
		return m_RenderSystem->LoadTexture(filename);
	}
	else
		return -1;
}




