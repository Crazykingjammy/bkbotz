#ifndef __Bitmap_2D_h__
#define __Bitmap_2D_h__

#include "./../Render/Direct3D/D3DLibs.h"

class CBitmap2D
{

public:

	CBitmap2D(IDirect3DDevice9 * device, LPCSTR  filename);
	~CBitmap2D();
		
	D3DXIMAGE_INFO      m_ImageInfo;
	LPCSTR				m_Filename;
	ID3DXSprite*		m_pSprite;
	IDirect3DTexture9*	m_Texture;

	//Draw ME
	void Draw(float X, float Y);
	void Draw(float X, float Y, float Z);

};
	
	

#endif