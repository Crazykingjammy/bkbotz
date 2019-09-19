#ifndef __Text_h__
#define __Text_h__

#include "../D3DLibs.h"
#include "../Color/Color.h"

class Text
{

public:

	HDC hdc;
	HFONT hFont;
	HFONT hFontOld;
	IDirect3DDevice9* Device;

	ID3DXMesh* m_Text;

	LOGFONT lf;
	

	void InitializeText();
	void LoadText(IDirect3DDevice9 * IDevice, LPCSTR name);
	void Display(float X, float Y, float Z);

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	

	Text();
	~Text();

};


#endif