#include "D3DMain.h"


void CD3DRender::SetParams(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hWnd)
{
	//Create the manager.
	m_ModelManager = new CModelManager(m_Device);

	//Create the Bitmap Manager
	m_BitmapManager = new CBitmapManager();

	//Get instance.
	pRenderInterface = CRenderInterface::GetInstance();

	m_Device->SetRenderState(D3DRS_LIGHTING, true);
	m_Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_Device->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, true);

	// Tex filtering.
	m_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	// Specify that the texture tiles.
	m_Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);



	//Clear this out please.
	D3DXMatrixIdentity(&m_ViewProjection);
	D3DXMatrixIdentity(&m_Projection);
	D3DXMatrixIdentity(&m_View);



}


int CD3DRender::LoadModel(char* filename)
{
	return (int) m_ModelManager->AddModel(filename);
}

int CD3DRender::LoadTexture(char* filename)
{
	CLabel fuckingneeded(filename);

	return (int) m_BitmapManager->Load2D(m_Device,&fuckingneeded);
}

D3DLIGHT9 CD3DRender::SetFromCLight(CLight light)
{

	D3DLIGHT9 rlight;
	ZeroMemory(&rlight, sizeof(D3DLIGHT9));

	//Assign Ambient
	rlight.Ambient.r = light.m_Ambient.r;
	rlight.Ambient.g = light.m_Ambient.g;
	rlight.Ambient.b = light.m_Ambient.b;
	rlight.Ambient.a = light.m_Ambient.a;

	//Assign Diffuse
	rlight.Diffuse.r = light.m_Diffuse.r;
	rlight.Diffuse.g = light.m_Diffuse.g;
	rlight.Diffuse.b = light.m_Diffuse.b;
	rlight.Diffuse.a = light.m_Diffuse.a;

	//Assign Specular
	rlight.Specular.r = light.m_Specular.r;
	rlight.Specular.g = light.m_Specular.g;
	rlight.Specular.b = light.m_Specular.b;
	rlight.Specular.a = light.m_Specular.a;

	//Assign Position
	rlight.Position.x = light.m_MyBody.centerPosition.x;
	rlight.Position.y = light.m_MyBody.centerPosition.y;
	rlight.Position.z = light.m_MyBody.centerPosition.z;

	//Assign the Direction
	rlight.Direction.x = light.m_Direction.x;
	rlight.Direction.y = light.m_Direction.y;
	rlight.Direction.z = light.m_Direction.z;


	//Assign Bitches
	rlight.Range		= light.m_Range;
	rlight.Falloff		= light.m_Falloff;
	rlight.Attenuation0 = light.m_Attenuation0;
	rlight.Attenuation1 = light.m_Attenuation1;
	rlight.Attenuation2 = light.m_Attenuation2;
	rlight.Phi			= light.m_Phi;
	rlight.Theta		= light.m_Theta;

	//Set the Light Type
	switch (light.m_Type)
	{
	case 0:			//Point light
		rlight.Type = D3DLIGHT_POINT;
		break;
	case 2:			//Directional light
		rlight.Type = D3DLIGHT_DIRECTIONAL;
		break;
	case 4:
		rlight.Type = D3DLIGHT_SPOT;
		break;
	}

	return rlight;

}

void CD3DRender::UpdateCamera(CBaseCamera* cam)
{

	//Look At

	D3DXVECTOR3 P;
	D3DXVECTOR3 L;
	D3DXVECTOR3 U;
	D3DXVECTOR3 R;

	P.x = cam->m_MyBody.centerPosition.x;
	P.y = cam->m_MyBody.centerPosition.y;
	P.z = cam->m_MyBody.centerPosition.z;

	L.x = cam->m_Look.x;
	L.y = cam->m_Look.y;
	L.z = cam->m_Look.z;

	U.x = cam->m_Up.x;
	U.y = cam->m_Up.y;
	U.z = cam->m_Up.z;

	R.x = cam->m_Right.x;
	R.y = cam->m_Right.y;
	R.z = cam->m_Right.z;

	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&L, &L);

	D3DXVec3Cross(&U, &L, &R);
	D3DXVec3Normalize(&U, &U);

	D3DXVec3Cross(&R, &U, &L);
	D3DXVec3Normalize(&R, &R);

	float x = -D3DXVec3Dot(&P, &R);
	float y = -D3DXVec3Dot(&P, &U);
	float z = -D3DXVec3Dot(&P, &L);


	cam->m_MyBody.centerPosition.x = P.x ;
	cam->m_MyBody.centerPosition.x = P.x ;
	cam->m_MyBody.centerPosition.x = P.x ;

	cam->m_Look.x = L.x;
	cam->m_Look.y = L.y;
	cam->m_Look.z = L.z;

	cam->m_Up.x = U.x;
	cam->m_Up.y = U.y;
	cam->m_Up.z = U.z;

	cam->m_Right.x = R.x;
	cam->m_Right.y = R.y;
	cam->m_Right.z = R.z;


	//Assign the View
	m_View(0,0) = cam->m_Right.x; 
	m_View(1,0) = cam->m_Right.y; 
	m_View(2,0) = cam->m_Right.z; 
	m_View(3,0) = x;   

	m_View(0,1) = cam->m_Up.x;
	m_View(1,1) = cam->m_Up.y;
	m_View(2,1) = cam->m_Up.z;
	m_View(3,1) = y;  

	m_View(0,2) = cam->m_Look.x; 
	m_View(1,2) = cam->m_Look.y; 
	m_View(2,2) = cam->m_Look.z; 
	m_View(3,2) = z;   

	m_View(0,3) = 0.0f;
	m_View(1,3) = 0.0f;
	m_View(2,3) = 0.0f;
	m_View(3,3) = 1.0f;

	
	m_ViewProjection = m_View * m_Projection;

}

void CD3DRender::LoadXFiles()
{
	//Load the world
	pRenderInterface->m_World->ModelIndex = m_ModelManager->AddModel(pRenderInterface->m_World->m_ModelName);

	// Go trough the list of objects an load thair x files.
	for(unsigned int i = 0; i < pRenderInterface->m_ObjectList.size(); i++)
	{
		pRenderInterface->m_ObjectList[i]->ModelIndex = m_ModelManager->AddModel(pRenderInterface->m_ObjectList[i]->m_ModelName);
	}
}


void CD3DRender::SwitchCamera()
{
	//Set the Projection.
	D3DXMatrixPerspectiveFovLH(&m_Projection,
		pRenderInterface->m_ActiveCamera->m_Fov,
		pRenderInterface->m_ActiveCamera->m_Aspect,
		pRenderInterface->m_ActiveCamera->m_Near,
		pRenderInterface->m_ActiveCamera->m_Far);

	m_Device->SetTransform(D3DTS_VIEW,&m_View);
	m_Device->SetTransform(D3DTS_PROJECTION,&m_Projection);

}

void CD3DRender::Load2DFiles()
{
	//AAAAAAAAAAAAAHHAHAHAHAHHA im on a COMMENTING SPREEEEEEEEEE!!!!!!!!!! get a gun type fun.
	for(unsigned int i = 0; i < pRenderInterface->m_Objects2D.size(); i++)
	{
		pRenderInterface->m_Objects2D[i]->ModelIndex = m_BitmapManager->Load2D(m_Device,
			pRenderInterface->m_Objects2D[i]);
	}
}

void CD3DRender::RenderParticles()
{
	//Get number of emmiters to create the number of vertex buffers
	unsigned int numEmitters = (unsigned int)pRenderInterface->m_ParticleList.size();

	//Might as well exit if we dont have any emitters right..... riiiiiigghhtt?
	if(numEmitters <= 0)
		return;

	D3DXMATRIX World, Orientation,OrientationX, OrientationZ, Position;


	for(unsigned int i = 0; i < numEmitters; i++)
	{
		m_Device->SetRenderState(D3DRS_POINTSIZE, FtoDw(pRenderInterface->m_ParticleList[i]->m_ParticleSize));

		

		//Write Transformation
		D3DXMatrixTranslation(&Position,pRenderInterface->m_ParticleList[i]->GetX(),
			pRenderInterface->m_ParticleList[i]->GetY(),
			pRenderInterface->m_ParticleList[i]->GetZ() );

		//Transform
		//m_Device->SetTransform(D3DTS_WORLD,&World);


		//Write Orientation
		D3DXMatrixRotationY(&Orientation,pRenderInterface->m_ParticleList[i]->m_RenderingOrientation.y);
		D3DXMatrixRotationX(&OrientationX,pRenderInterface->m_ParticleList[i]->m_RenderingOrientation.x);
		D3DXMatrixRotationZ(&OrientationZ,pRenderInterface->m_ParticleList[i]->m_RenderingOrientation.z);


		


		if(!pRenderInterface->m_ParticleList[i]->m_World)
		{
			//Get the world.
			World = Orientation * OrientationX * OrientationZ * Position ;
		}

		//Transform
		m_Device->SetTransform(D3DTS_WORLD,&World);

		//If we arent active skip the drawing.
		if(!pRenderInterface->m_ParticleList[i]->m_Active)
			continue;

		//Get number o particles.
		int numParticles = pRenderInterface->m_ParticleList[i]->GetNumberParticles();

		//Why such a hassle....
		m_Device->SetTexture(0,
			m_BitmapManager->m_Bitmaps[pRenderInterface->m_ParticleList[i]->ModelIndex]->GetD3DTexture());

		//STEAM a STREAM for MEANGREEN HERRBB
		m_Device->SetStreamSource(0,m_VertexBuffer[i],0,sizeof(ParticleVertex));

		//No i will hard code it.
		m_Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

		//Yes finally.. the FUCKING DRAWING! sorry.. im sick remember :-(
		m_Device->DrawPrimitive(D3DPT_POINTLIST,0,numParticles);

	}


}


void CD3DRender::DisplayParticles()
{
	//FIll up that Vertex buffer bitches.
	ConstructParticleVertexBuffer();

	//Set States//
	m_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_Device->SetRenderState(D3DRS_POINTSCALEENABLE, true); 
	
	
	m_Device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	// control the size of the particle relative to distance
	m_Device->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	m_Device->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	m_Device->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// use alpha from texture
	m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);



	//Ahhhhhhhh im sick. :-(
	RenderParticles();

	m_Device->SetRenderState(D3DRS_LIGHTING,          true);
	m_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_Device->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
	m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);

}


void CD3DRender::InitalizeParticles()
{
	//Get number of emmiters to create the number of vertex buffers
	unsigned int numEmitters = (unsigned int)pRenderInterface->m_ParticleList.size();

	//Might as well exit if we dont have any emitters right..... riiiiiigghhtt?
	if(numEmitters <= 0)
		return;

	//If the buffer wasent created yet... create one....
	if(!m_VertexBuffer)
	{
		//Create an array of buffers;
		m_VertexBuffer = new LPDIRECT3DVERTEXBUFFER9[numEmitters];
	}

	//Yeah.
	HRESULT hr;

	for(unsigned int i = 0; i < numEmitters; i++)
	{

		//Get number o particles.
		int numParticles = pRenderInterface->m_ParticleList[i]->GetNumberParticles();

		//Create the vertex buffer
		hr = m_Device->CreateVertexBuffer(numParticles * sizeof(ParticleVertex),D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
			D3DFVF_XYZ | D3DFVF_DIFFUSE,D3DPOOL_DEFAULT,&m_VertexBuffer[i],0);

		//Yes yes i will do some saftey checks... damn checks... check for meee pleaassee insanah fucking T, bitches.. semicolon;
		if(FAILED(hr))
		{
			::MessageBox(0, "CreateVertexBuffer() - FAILED", "PSystem", 0);
			return;
		}

		//Im  to lazy to create a load bitmap that dosent take a C label hehehehhe ....fuck offf....... Create one.
		CLabel FuckingNeededAgain(pRenderInterface->m_ParticleList[i]->m_ModelName);
		pRenderInterface->m_ParticleList[i]->ModelIndex = m_BitmapManager->Load2D(m_Device,&FuckingNeededAgain);


	}


}

void CD3DRender::ConstructParticleVertexBuffer()
{
	//////////////////////////////////////////////////////////////////////////
	// Load Vertex Buffer with particles
	//////////////////////////////////////////////////////////////////////////


	//Get number of emmiters to create the number of vertex buffers
	unsigned int numEmitters = (unsigned int)pRenderInterface->m_ParticleList.size();

	//Might as well exit if we dont have any emitters right..... riiiiiigghhtt?
	if(numEmitters <= 0)
		return;

	for(unsigned int i = 0; i < numEmitters; i++)
	{
		ParticleVertex *pPointVertices;

		//Get number o particles.
		int numParticles = pRenderInterface->m_ParticleList[i]->GetNumberParticles();

		//Lock the buffer. ... Locking Key, Bitches....
		m_VertexBuffer[i]->Lock(0,numParticles * sizeof(ParticleVertex),(void**)&pPointVertices, D3DLOCK_DISCARD);

		for(int j = 0; j < numParticles; j++)
		{
			pPointVertices->position.x	= pRenderInterface->m_ParticleList[i]->m_Particles[j].position.x;
			pPointVertices->position.y	= pRenderInterface->m_ParticleList[i]->m_Particles[j].position.y;
			pPointVertices->position.z	= pRenderInterface->m_ParticleList[i]->m_Particles[j].position.z;

			pPointVertices->color.r		= pRenderInterface->m_ParticleList[i]->m_Particles[j].color.r;
			pPointVertices->color.g		= pRenderInterface->m_ParticleList[i]->m_Particles[j].color.g;
			pPointVertices->color.b		= pRenderInterface->m_ParticleList[i]->m_Particles[j].color.b;
			pPointVertices->color.a		= pRenderInterface->m_ParticleList[i]->m_Particles[j].color.a;
			pPointVertices++;
		}

		//Unlock cos we got the locking keeeeyyyy.
		m_VertexBuffer[i]->Unlock();

	}


	//////////////////////////////////////////////////////////////////////////
	// Load Vertex Buffer with particles
	//////////////////////////////////////////////////////////////////////////

}

