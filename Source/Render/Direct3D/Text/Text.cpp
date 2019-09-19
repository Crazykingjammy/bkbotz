#include "Text.h"


const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );



const int Width  = 640;
const int Height = 480;


Text::Text()
{


}

Text::~Text()
{
}

void Text::InitializeText()
{
	
//	m_Text = 0;
//	Device = 0;
	hFont = 0;
	hFontOld = 0;
}

void Text::LoadText(IDirect3DDevice9 * IDevice, LPCSTR name)
{

	Device = IDevice;
	//
	// Framework functions

	// Get a handle to a device context.
	//

	hdc = CreateCompatibleDC( 0 );

	//
	// Describe the font we want.
	//

	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight         = 25;    // in logical units
	lf.lfWidth          = 12;    // in logical units
	lf.lfEscapement     = 0;        
	lf.lfOrientation    = 0;     
	lf.lfWeight         = 500;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic         = false;   
	lf.lfUnderline      = false;    
	lf.lfStrikeOut      = false;    
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = 0;              
	lf.lfClipPrecision  = 0;          
	lf.lfQuality        = 0;           
	lf.lfPitchAndFamily = 0;    
	strcpy(lf.lfFaceName, "Times New Roman"); // font style

	//
	// Create the font and select it with the device context.
	//
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont); 

	//
	// Create the text mesh based on the selected font in the HDC.
	//
	D3DXCreateText(Device, hdc, name, 
		0.001f, 0.4f, &m_Text, 0, 0);

	//
	// Restore the old font and free the acquired HDC.
	//
	SelectObject(hdc, hFontOld);
	DeleteObject( hFont );
	DeleteDC( hdc );

}

void Text::Display(float X, float Y, float Z)
{
	
	D3DXMATRIX T;


	//
	// Render
	//

	D3DXMatrixTranslation(&T, X, Y, Z);

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);

	Device->SetMaterial(&RED_MTRL);
	m_Text->DrawSubset(0);

}

D3DMATERIAL9 Text::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

D3DLIGHT9 Text::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

