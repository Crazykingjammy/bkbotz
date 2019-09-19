#include "2DOverlayManager.h"

CBitmapManager::CBitmapManager()
{
	//Get Instance
	pRenderInterface = CRenderInterface::GetInstance();
}

CBitmapManager::~CBitmapManager()
{

}

unsigned int CBitmapManager::Load2D(IDirect3DDevice9* device, CLabel* label)
{

	//Go trough list.. if we are already loaded...
	for(unsigned i = 0; i < m_Bitmaps.size(); i++)
	{
		if(!strcmp(m_Bitmaps[i]->m_Filename,label->m_ModelName))
			return i;								//...Return the Index.
	}

	//Create Bitmap
	CBitmaps2D *bitmap;
	bitmap = new CBitmaps2D(device,label);

	//Add to Vector
	m_Bitmaps.push_back(bitmap);

	//Return the Index.
	return ( (unsigned int)m_Bitmaps.size() - 1);
}

void CBitmapManager::Draw2DImages(IDirect3DDevice9* device)
{
	for(unsigned int i = 0; i < pRenderInterface->m_Objects2D.size(); i++ )
	{
		if(pRenderInterface->m_Objects2D[i]->m_Visible)
		{
			//Store the Index.
			int index = pRenderInterface->m_Objects2D[i]->ModelIndex;

			m_Bitmaps[index]->Draw(pRenderInterface->m_Objects2D[i]);
		}
	}

}


void CBitmapManager::ClearImages()
{
	if(m_Bitmaps.empty())
		return;

	for(unsigned int i = 0; i < m_Bitmaps.size(); i++)
	{
		delete m_Bitmaps[i];
	}

	m_Bitmaps.clear();
}