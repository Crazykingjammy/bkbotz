#ifndef __D3D_MAIN_H__
#define __D3D_MAIN_H__

#include "..//Render.h"

#include "Text/Text.h"
#include "Parimitives.h"
#include "../RenderInterface.h"
#include "Models/ModelManager.h"
#include "2D/2DOverlayManager.h"


//Basic particle information.
struct ParticleVertex
{
	D3DXVECTOR3	position;
	D3DXCOLOR	color;
};

class CD3DRender : public CRender
{
private:
	D3DDEVTYPE deviceType;
	IDirect3DDevice9 * m_Device;
	IDirect3D9*		d3d9;
	HRESULT hr;
	HWND hwnd;
	
	
	//For Shadows.
	float rad;
	ID3DXMesh* Sphere;		

	//Lets know about the object manager.
	CRenderInterface* pRenderInterface;
	
	CModelManager	*m_ModelManager;

	CBitmapManager	*m_BitmapManager;


	//Matrices for view.
	D3DXMATRIX m_View, m_Projection, m_ViewProjection;

	//Array of Vertex buffers to store all the particles in.
	LPDIRECT3DVERTEXBUFFER9* m_VertexBuffer;

	//Extra debugging shit that should be deleted.

public:

	Text Test;



	//functions 

public:

	//Constructor and Destructor
	CD3DRender()
	{
		m_VertexBuffer = NULL;
	}
	~CD3DRender()
	{


	}

	//Functions.

	//Initalize Direct 3D
	void Initalize(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hwnd);

	//This is the Display function to do all the drawing basicaly.
	void Update(float timeDelta);

	//Any Shutdown we need here. 
	void Shutdown();

	//Create Scene
	void CreateScene();

	//Destroy Scene();
	void DestoryScene();

	//Switch Camera
	void SwitchCamera();

	//Load Model
	int LoadModel(char* filename);

	//Load Texture
	int LoadTexture(char* filename);

private:

	//Creates direct 3d
	void SetParams(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hWnd);

	//Init the particles here.
	void InitalizeParticles();

	//Create particles and everything needed.
	void ConstructParticleVertexBuffer();

	//d3d Functions
	void LoadXFiles();

	//load the 2d objects
	void Load2DFiles();

	//Display objects.
	void Display();

	//Display 2d objects.
	void Draw2D();

	//Display particles
	void DisplayParticles();

	//Do the rendering for the particles
	void RenderParticles();

	//Assign a Light
	D3DLIGHT9 SetFromCLight(CLight light);

	//Build the current camera.
	void UpdateCamera(CBaseCamera* cam);

	void DrawStencilShadows();

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	DWORD FtoDw(float f)
	{
		return *((DWORD*)&f);
	}

};



#endif
