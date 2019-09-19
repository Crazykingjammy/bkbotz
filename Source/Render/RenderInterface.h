#ifndef __Render_Interface_h__
#define __Render_Interface_h__

#include "Render.h"
#include "../Object/Object.h"
#include "Lights/Lights.h"
#include "Camera/Camera.h"
#include "Camera/FreeCamera.h"
#include "../Object/2D/2d.h"
#include "../Object/Particles/Particle.h"

#define MAX_LIGHTS 4

class CRenderInterface
{

private:
	// Instance
	static CRenderInterface *pInstance;

	//Default Constructor
	CRenderInterface();
	//Copy Constructor
	CRenderInterface(const CRenderInterface&);
	//Assignment Operator
	CRenderInterface &operator = (const CRenderInterface&);

	//Render system
	CRender*  m_RenderSystem;


public:

	//List of objects.
	vector<CBaseObject*> m_ObjectList;

	//List of 2D Objects.
	vector<CLabel*> m_Objects2D;

	//List of particles
	vector <CBaseParticleEmitter*> m_ParticleList;

	//Lights
	CLight Light[MAX_LIGHTS];

	//Pointer to the Active Camera
	CBaseCamera* m_ActiveCamera;

	//Pointer to the world.
	CWorld* m_World;

	//gosh.. ill put a default camera..........gosh
	CFreeCamera m_DefaultCamera_GOSH;

	CWorld* m_DefaultWorld;


public:
	static void DeleteInstance(void);
	static CRenderInterface* GetInstance(void);

public:

	//Public Interface.

	//Initialize
	void Initialize(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hwnd);

	//Update
	void Update(float timeDelta);

	//Shutdown
	void Shutdown();

	//Add an object to the list.
	void AddObject(CBaseObject* object);

	//Add a label on the screen.
	void AddLabel(CLabel* label);

	//Add a particle to the list.
	void AddParticle(CBaseParticleEmitter* emitter);

	//Load a model to memory
	int LoadModel(char* filename);

	//Load a texture to memory
	int LoadTexture(char *filename);

	//Set to the current active camera.
	void SetActiveCamera(CBaseCamera* camera);

	//Set the world
	void SetWorld(LPCSTR worldmodel);

	//Create the Scene
	void CreateScene();

	//Destroy the Scene;
	void DestroyScene();




};






#endif