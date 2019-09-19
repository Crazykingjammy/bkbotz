#ifndef __Color_h__
#define __Color_h__

#include "../D3DLibs.h"

namespace Color
{


	

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);	


	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );

	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	/*const D3DMATERIAL9 Material::WHITE_MTRL() {  return InitMtrl(Col.WHITE(), Col.WHITE(), Col.WHITE(), Col.BLACK(), 2.0f); }
	const D3DMATERIAL9 Material::RED_MTRL()   {  return InitMtrl(Col.RED(), Col.RED(), Col.RED(), Col.BLACK(), 2.0f); }
	const D3DMATERIAL9 Material::GREEN_MTRL() {  return InitMtrl(Col.GREEN(), Col.GREEN(), Col.GREEN(), Col.BLACK(), 2.0f); }
	const D3DMATERIAL9 Material::BLUE_MTRL()  {  return InitMtrl(Col.BLUE(), Col.BLUE(), Col.BLUE(), Col.BLACK(), 2.0f); }
	const D3DMATERIAL9 Material::YELLOW_MTRL(){  return InitMtrl(Col.YELLOW(), Col.YELLOW(), Col.YELLOW(), Col.BLACK(), 2.0f); }*/
	/**************   Colors   ***************/	
	
	/*static const D3DXCOLOR      WHITE( );
	static const D3DXCOLOR      BLACK( );
	static const D3DXCOLOR        RED( );
	static const D3DXCOLOR      GREEN( );
	static const D3DXCOLOR       BLUE( );
	static const D3DXCOLOR     YELLOW( );
	static const D3DXCOLOR       CYAN( );
	static const D3DXCOLOR    MAGENTA( );*/

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