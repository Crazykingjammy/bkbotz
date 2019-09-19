#ifndef __Bitmaps_2D_h__
#define __Bitmaps_2D_h__

#include "../D3DLibs.h"
#include "../../../Object/2D/2D.h"

class CBitmaps2D
{

public:

	CBitmaps2D(IDirect3DDevice9 * device, CLabel* label);
	~CBitmaps2D();
		
	D3DXIMAGE_INFO      m_ImageInfo;
	char				m_Filename[256];
	ID3DXSprite*		m_pSprite;
	IDirect3DTexture9*	m_Texture;

	//Draw ME
	void Draw(CLabel* label);

	//Sure why not... for particles.
	IDirect3DTexture9* GetD3DTexture() {return m_Texture;}

private:
	//Long ass function for displaying text.
	void DrawText(CLabel* label);

};
	
	

#endif