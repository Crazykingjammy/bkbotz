#ifndef __2D_Manager_h__
#define __2D_Manager_h__

#include "2DOverlay.h"
#include "../../RenderInterface.h"

class CBitmapManager
{
public:

	//List of 2D Objects
	vector<CBitmaps2D*> m_Bitmaps;

	//Instance to the Render Interface
	CRenderInterface* pRenderInterface;

public:

	//Methods
	CBitmapManager();
	~CBitmapManager();

	//Load the files.
	unsigned int Load2D(IDirect3DDevice9* device, CLabel* label);

	//Draw the list of 2d Objects.
	void Draw2DImages(IDirect3DDevice9* device);

	//Delete all of the Images
	void ClearImages();
};


#endif