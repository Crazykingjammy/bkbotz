#include "modelmanager.h"

CModelManager::CModelManager(IDirect3DDevice9* device)
{
	m_Device = device;

	//Get Instance
	m_RenderInterface = CRenderInterface::GetInstance();

}

CModelManager::~CModelManager()
{

}

unsigned int CModelManager::AddModel(char* filename)
{
	// if already in list
	for(unsigned int i = 0; i < m_Models.size(); i++)
	{
		if(!strcmp(m_Models[i]->m_Filename,filename))
			return i;				//... Return the index
	}

	CModel *model;
	model = new CModel;

	//Load the file.
	model->Load(m_Device,filename);

	//Add to the list
	m_Models.push_back(model);

	//Return the index
	return ( (unsigned int)m_Models.size() - 1);
}

void CModelManager::DeleteModels()
{
	if(m_Models.empty())
		return;

	for( unsigned int i = 0; i < m_Models.size(); i++)
	{
		//Clean up the mesh
		m_Models[i]->Delete();

		//Delete Cmodel
		delete m_Models[i];
	}

	m_Models.clear();
}

void CModelManager::DrawModels()
{
	//Draw the worlds model at its position.
	//m_Models[m_RenderInterface->m_World->ModelIndex]->Display(m_Device,m_RenderInterface->m_World);

	for(unsigned int i = 0; i < m_RenderInterface->m_ObjectList.size(); i++)
	{
		if(m_RenderInterface->m_ObjectList[i]->m_Visible)
		{
			//For Readibility sake
			int index = m_RenderInterface->m_ObjectList[i]->ModelIndex;

			m_Models[index]->Display(m_Device,m_RenderInterface->m_ObjectList[i]);
		}
	}
}