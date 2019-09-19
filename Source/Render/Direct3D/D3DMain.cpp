
#include "D3dMain.h"




void CD3DRender::Initalize(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hwnd)
{
	deviceType = D3DDEVTYPE_HAL;
	
	
	
	//Init D3D Bitches.

	// Step 1: Create the IDirect3D9 object.
	/**********************************************************/
	d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if( !d3d9 )
	{
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
	}
	/**********************************************************/

	/*

	//// Step 2: Check for hardware vp.
	///**********************************************************/
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	///**********************************************************/

	//// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.
	///**********************************************************/
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = Width;
	d3dpp.BackBufferHeight           = Height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = !Fullscreen;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_RATE_DEFAULT;
	///**********************************************************/



	//// Step 4: Create the device.
	///**********************************************************/
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&m_Device);         // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&m_Device);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			
		}
	}

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)8);
	m_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	



	//d3d9->Release(); // done with d3d9 object
	/**********************************************************/


	//Set things needed for rendering.
	SetParams(hInstance,Width,Height,Fullscreen,hwnd);

	Sphere = 0;
	rad = .9f;
	D3DXCreateSphere(m_Device, rad, 15, 15, &Sphere, 0);	

	
}



// Update the Camera 
void CD3DRender::Update(float timeDelta)
{
	UpdateCamera(pRenderInterface->m_ActiveCamera);

	m_Device->SetTransform(D3DTS_VIEW,&m_View);

	Display();
}

//Display shit mother fucker.
void CD3DRender::Display()
{

	m_Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xaaaaaaaa, 1.0f, 0);
	
	m_Device->BeginScene();


	//Set the mother fucking lights.
	for(unsigned int i = 0; i < MAX_LIGHTS; i++)
	{
		if(pRenderInterface->Light[i].m_Active == true)
		{
			D3DLIGHT9 light;
			light = SetFromCLight(pRenderInterface->Light[i]);
			m_Device->SetLight(i,&light);
		}
	}


	//Draw World.
	m_ModelManager->m_Models[pRenderInterface->m_World->ModelIndex]->Display(m_Device,pRenderInterface->m_World);

	DrawStencilShadows();

	m_ModelManager->DrawModels();
	m_BitmapManager->Draw2DImages(m_Device);

	DisplayParticles();

	m_Device->EndScene();

	m_Device->Present(0, 0, 0, 0);


}





void CD3DRender::CreateScene()
{


	//////////////////////////////////////////////////////////////////////////
	//	Create Lights
	//////////////////////////////////////////////////////////////////////////
	

	for(unsigned int i = 0; i < MAX_LIGHTS; i++)
	{
		if(pRenderInterface->Light[i].m_Active == true)
		{
			D3DLIGHT9 light;
			light = SetFromCLight(pRenderInterface->Light[i]);
			m_Device->SetLight(i,&light);
			m_Device->LightEnable(i,true);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	Create Lights
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Current Camera Setup
	//////////////////////////////////////////////////////////////////////////
	

	D3DXMatrixPerspectiveFovLH(&m_Projection,
		pRenderInterface->m_ActiveCamera->m_Fov,
		pRenderInterface->m_ActiveCamera->m_Aspect,
		pRenderInterface->m_ActiveCamera->m_Near,
		pRenderInterface->m_ActiveCamera->m_Far);

	m_Device->SetTransform(D3DTS_VIEW,&m_View);
	m_Device->SetTransform(D3DTS_PROJECTION,&m_Projection);
	//////////////////////////////////////////////////////////////////////////
	// Current Camera Setup
	//////////////////////////////////////////////////////////////////////////


	//Load up objects dammit!
	LoadXFiles();

	//Load the fucking 2 fuckin d files. .. just incase anybody is reading this :-) i have fun.
	Load2DFiles();

	//Create those damn particles boy!!!!!!!! play geometry wars!! NOW! get up now.. stop fucking reading this you fuck..
	//Load those particle images...
	InitalizeParticles();

}

void CD3DRender::DrawStencilShadows()
{
	//	Ball = cBall;

	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	D3DXVECTOR4 lightDirection(0.7f, -1.0f, 0.07f, 0.0f);
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);

	D3DXMATRIX S;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);


	// alpha blend the shadow
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 BLACK_MTRL  = InitMtrl(BLACK, BLACK, BLACK, BLACK, 0.0f);
	BLACK_MTRL.Diffuse.a = 0.5f; // 50% transparency.

	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	m_Device->SetRenderState(D3DRS_ZENABLE, false);



	//Go trough all the lights
	for(int light = 0; light < 4; light++)
	{

		//if we cast a shadow draw it.. else dont.
		if(pRenderInterface->Light[light].m_CastShadow && pRenderInterface->Light[light].m_Active )
		for(unsigned int i = 0; i < pRenderInterface->m_ObjectList.size(); i++)
		{
			if(pRenderInterface->m_ObjectList[i]->m_Visible && pRenderInterface->m_ObjectList[i]->m_DrawShadow)
			{
				//If dynamic shadow based on light.
				if(true)
				{
					//Recalculate shadow matrix
					CVector3f L = pRenderInterface->m_ObjectList[i]->m_MyBody.centerPosition - pRenderInterface->Light[light].m_MyBody.centerPosition;
					L.Normalize();

					//Assign the Light.
					lightDirection.x = L.x;
					lightDirection.y = L.y;
					lightDirection.z = L.z;

					D3DXMatrixShadow(
						&S,
						&lightDirection,
						&groundPlane);

				}


				float ScaleValue = pRenderInterface->m_ObjectList[i]->m_MyBody.radius;

				D3DXMATRIX T, Scale;
				D3DXMatrixTranslation(
					&T,
					pRenderInterface->m_ObjectList[i]->GetX(),
					pRenderInterface->m_ObjectList[i]->GetY(),
					pRenderInterface->m_ObjectList[i]->GetZ());

				D3DXMatrixScaling(&Scale,ScaleValue,ScaleValue,ScaleValue);

				D3DXMATRIX W = Scale * T * S;
				m_Device->SetTransform(D3DTS_WORLD, &W);

				m_Device->SetMaterial(&BLACK_MTRL);
				m_Device->SetTexture(0, 0);
				//Sphere->DrawSubset(0);
		
				//Shadow for the model.
				for(unsigned int j = 0; j < m_ModelManager->m_Models[pRenderInterface->m_ObjectList[i]->ModelIndex]->m_Materials.size(); j++ )
				{
					m_ModelManager->m_Models[pRenderInterface->m_ObjectList[i]->ModelIndex]->m_Mesh->DrawSubset(j);
				}
			
			
			}
		
		
		}
	}

	//Reset the rendering states.
	m_Device->SetRenderState(D3DRS_ZENABLE, true);
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_Device->SetRenderState(D3DRS_STENCILENABLE,    false);





}

D3DMATERIAL9 CD3DRender::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

//And Delete SHit too.
void CD3DRender::Shutdown()
{
	//Cleanup X-file Manager.
	m_ModelManager->DeleteModels();
	delete m_ModelManager;

	m_BitmapManager->ClearImages();
	delete m_BitmapManager;
	
//	teapot.Cleanup();


	//Release the object.
	m_Device->Release();
	d3d9->Release();
}






void CD3DRender::DestoryScene()
{
	//Create default light.
	CLight light;
	
	//Set it equal to 4 lights in d3d
	D3DLIGHT9 dlight;
	dlight = SetFromCLight(light);
	for(int i = 0; i < 4; i++)
	{
		m_Device->SetLight(i,&dlight);
		m_Device->LightEnable(i,false);
	}

	//If there is particles
	if(m_VertexBuffer)
	{
		//Destroy the particles
		unsigned int numEmitters = (unsigned int)pRenderInterface->m_ParticleList.size();

		//Clean up the vertex buffers
		for(unsigned int i = 0; i< numEmitters; i++)
		{
			m_VertexBuffer[i]->Release();
		}

		//Nullify it.
		m_VertexBuffer = NULL;
	}

	//Erase all models in the scene
	m_ModelManager->DeleteModels();

	//Free all the 2d images
	m_BitmapManager->ClearImages();

}



