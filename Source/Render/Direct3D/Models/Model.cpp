#include "model.h"


CModel::CModel()
{
	m_Mesh = 0;
}

CModel::~CModel()
{

}

void CModel::Load(IDirect3DDevice9* Device, char* Filename)
{
	//Set the filename for Identifier.
	//m_Filename = Filename;


	strcpy(m_Filename,Filename);

	HRESULT hr				= 0;
	ID3DXBuffer* adjBuffer	= 0;
	ID3DXBuffer* mtrlBuffer = 0;
	DWORD        numMtrls	= 0;



	//Load the xfile here bitch.
	hr = D3DXLoadMeshFromX(m_Filename,D3DXMESH_MANAGED,Device,
		&adjBuffer,&mtrlBuffer,0,&numMtrls,&m_Mesh);

	if (FAILED(hr))
	{
		::MessageBox(0,"D3D Load XFile Failed!","Load Error",0);
	}


	//
	// Extract the materials, load textures.
	//

	if( mtrlBuffer != 0 && numMtrls != 0 )
	{
		D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		//D3DMATERIAL9

		for(int i = 0; i < (signed)numMtrls; i++)
		{
			// the MatD3D property doesn't have an ambient value set
			// when its loaded, so set it now:
			//mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

			// save the ith material
			m_Materials.push_back( mtrls[i].MatD3D );

			// check if the ith material has an associative texture
			if( mtrls[i].pTextureFilename != 0 )
			{
				// yes, load the texture for the ith subset
				IDirect3DTexture9* tex = 0;

				//Add the models/ extention to the texture.
				char filepath[128] = "ModelTextures/";
				char file[128];
				strcpy(file,mtrls[i].pTextureFilename);
				strcat(filepath,file);


				D3DXCreateTextureFromFileEx(
					Device,                   // device
					filepath,					// filename
					0, 0,                           // width & height
					0, 0,                           // mipmaps
					D3DFMT_UNKNOWN,                 // format
					D3DPOOL_MANAGED,                // memory pool
					D3DX_DEFAULT, D3DX_DEFAULT,     // filters
					D3DCOLOR_XRGB(255, 0, 255 ),   // key color
					NULL,							// returned image info
					NULL,                           // palette information
					&tex );   

				// save the loaded texture
				m_Textures.push_back( tex );
			}
			else
			{
				// no texture for the ith subset
				m_Textures.push_back( 0 );
			}
		}
	}

	Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer

	//Write triangles to file before optimization.
	//WriteTrianglesToFile();

	//
	// Optimize the mesh.
	//

	hr = m_Mesh->OptimizeInplace(		
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT  |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)adjBuffer->GetBufferPointer(),
		0, 0, 0);

	Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

	if(FAILED(hr))
	{
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
	}


	//
	// Set texture filters.
	//

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);




}

void CModel::Display(IDirect3DDevice9* Device, CBaseObject* object)
{
	if(Device)
	{
		D3DXMATRIX Position;
		D3DXMATRIX Scale;
		D3DXMATRIX Orientation;
		D3DXMATRIX OrientationX;
		D3DXMATRIX OrientationZ;
		D3DXMATRIX World;



		//Write Transformation
		D3DXMatrixTranslation(&Position,object->GetX(),object->GetY(),object->GetZ());

		//Write Orientation
		D3DXMatrixRotationY(&Orientation,object->m_RenderingOrientation.y);
		D3DXMatrixRotationX(&OrientationX,object->m_RenderingOrientation.x);
		D3DXMatrixRotationZ(&OrientationZ,object->m_RenderingOrientation.z);


		//Write Scale.
		D3DXMatrixScaling(&Scale,object->m_MyBody.radius,object->m_MyBody.radius,object->m_MyBody.radius);
	
		World = Scale * Orientation * OrientationX * OrientationZ * Position ;


		//Transform
		Device->SetTransform(D3DTS_WORLD,&World);

		//Bounding volume.
		if(object->m_DrawBoundingSphere)
		{
			LPD3DXMESH mesh;
			
			D3DXCreateSphere(Device,object->m_MyBody.radius,10,25,&mesh,NULL);

			mesh->DrawSubset(0);

			mesh->Release();
		}

		//
		// Render
		//

		// draw the mesh
		for(int i = 0; i < (signed)m_Materials.size(); i++)
		{
			Device->SetMaterial( &m_Materials[i] );
			Device->SetTexture(0, m_Textures[i]);
			m_Mesh->DrawSubset(i);
		}	

	}

}

void CModel::Delete()
{

	for(unsigned int i = 0; i < m_Textures.size(); i++)
		 Release(m_Textures[i]);

	m_Mesh->Release();

	//Clear the texture list.
	m_Textures.clear();

	//Clear Materials list.
	m_Materials.clear();



}

void CModel::WriteTrianglesToFile()
{
	void* VertexData;

	m_Mesh->LockVertexBuffer(D3DLOCK_READONLY,&VertexData);

	DWORD numverts;

	numverts = m_Mesh->GetNumVertices();


	FILE* pfile;
	pfile = fopen("triangle.txt","w");
	fwrite(VertexData,sizeof(float) * numverts  ,1,pfile);
	fclose(pfile);



	

}