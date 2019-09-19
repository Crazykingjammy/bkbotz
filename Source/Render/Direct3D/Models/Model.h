#ifndef __Model_h__
#define __Model_h__

#include "../../Direct3D/D3DLibs.h"
#include "../../../main.h"
#include "../../../Object/Object.h"

class CModel
{
public:

	// Name for File.
	char							m_Filename[256];

	vector<D3DMATERIAL9>			m_Materials;
	vector<IDirect3DTexture9*>		m_Textures;
	ID3DXMesh*						m_Mesh;


public:

	void Load(IDirect3DDevice9* Device, char* Filename);
	void Display(IDirect3DDevice9* Device, CBaseObject* object);
	void Delete();

	void WriteTrianglesToFile();

	CModel(void);
	~CModel(void);


	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}

	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}



};



#endif