#include "2DOverlay.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


CBitmap2D::CBitmap2D(IDirect3DDevice9 *device, LPCSTR filename)
{
	D3DXCreateSprite(device, &m_pSprite);

	D3DXCreateTextureFromFileEx(
		device,                   // device
		filename,						// filename
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,                           // width & height
		D3DX_DEFAULT, 0,                           // mipmaps
		D3DFMT_UNKNOWN,                 // format
		D3DPOOL_MANAGED,                // memory pool
		D3DX_DEFAULT, D3DX_DEFAULT,     // filters
		D3DCOLOR_XRGB( 255, 0, 255 ),   // key color
		&m_ImageInfo,                      // returned image info
		NULL,                           // palette information
		&m_Texture );               // returned texture
}

CBitmap2D::~CBitmap2D()
{
	m_Texture->Release();
}

void CBitmap2D::Draw(float X, float Y)
{

		if ( SUCCEEDED( m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK ) ) )
		{
			D3DXVECTOR3 a( m_ImageInfo.Width >> 1, m_ImageInfo.Height >> 1, 0.0f );

			m_pSprite->Draw(
				m_Texture,
				NULL,
				&a,
				&D3DXVECTOR3( X, Y, 0.0f ),
				0xffffffff );

			m_pSprite->End();

	}

}

void CBitmap2D::Draw(float X, float Y, float Z)
{

	if ( SUCCEEDED( m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK ) ) )
	{
		D3DXVECTOR3 a( m_ImageInfo.Width >> 1, m_ImageInfo.Height >> 1, 0.0f );

		m_pSprite->Draw(
			m_Texture,
			NULL,
			&a,
			&D3DXVECTOR3( X, Y, Z ),
			0xffffffff );

		m_pSprite->End();

	}

}



