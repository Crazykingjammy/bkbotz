#ifndef __Model_Manager_h__
#define __Model_Manager_h__


#include "Model.h"
#include "../../RenderInterface.h"
#include "../D3dLibs.h"
#include "../../../main.h"

class CModelManager
{
public:
	
	vector<CModel*> m_Models;
	IDirect3DDevice9* m_Device;

	CRenderInterface* m_RenderInterface;

public:

	CModelManager(IDirect3DDevice9* device);
	~CModelManager();

	//Add Model
	unsigned int AddModel(char* filename);

	//Draw all Models
	void DrawModels();

	//Shutdown
	void DeleteModels();



};




#endif