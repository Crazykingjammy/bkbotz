#include "Color.h"

D3DMATERIAL9 Color::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;

}

/***********************************************************************/
//
//const D3DXCOLOR Color::WHITE  () { return D3DCOLOR_XRGB(255, 255, 255); }
//const D3DXCOLOR Color::BLACK  () { return D3DCOLOR_XRGB(  0,   0,   0); }
//const D3DXCOLOR Color::RED    () { return D3DCOLOR_XRGB(255,   0,   0); }
//const D3DXCOLOR Color::GREEN  () { return D3DCOLOR_XRGB(  0, 255,   0); }
//const D3DXCOLOR Color::BLUE   () { return D3DCOLOR_XRGB(  0,   0, 255); }
//const D3DXCOLOR Color::YELLOW () { return D3DCOLOR_XRGB(255, 255,   0); }
//const D3DXCOLOR Color::CYAN   () { return D3DCOLOR_XRGB(  0, 255, 255); }
//const D3DXCOLOR Color::MAGENTA() { return D3DCOLOR_XRGB(255,   0, 255); }
//
///************************************************************************/
//
//
