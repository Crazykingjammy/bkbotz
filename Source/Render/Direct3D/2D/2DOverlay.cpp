#include "2DOverlay.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


CBitmaps2D::CBitmaps2D(IDirect3DDevice9 * device, CLabel* label)
{

	
	LPCSTR filename;
	filename = label->m_ModelName;

	D3DXCreateSprite(device, &m_pSprite);

	D3DXCreateTextureFromFileEx(
		device,                   // device
		filename,						// filename
		0, 0,                           // width & height
		0, 0,                           // mipmaps
		D3DFMT_UNKNOWN,                 // format
		D3DPOOL_MANAGED,                // memory pool
		D3DX_DEFAULT, D3DX_DEFAULT,     // filters
		D3DCOLOR_XRGB( 255, 0, 255 ),   // key color
		&m_ImageInfo,                      // returned image info
		NULL,                           // palette information
		&m_Texture );               // returned texture


	//Set the width.
	label->m_Rect.right = m_ImageInfo.Width;
	label->m_ImageWidth = m_ImageInfo.Width;

	//Set the height
	label->m_Rect.bottom = m_ImageInfo.Height;
	label->m_ImageHeight = m_ImageInfo.Height;
}

CBitmaps2D::~CBitmaps2D()
{
	m_Texture->Release();
	m_pSprite->Release();
}



void CBitmaps2D::Draw(CLabel* label)
{
	//Create a rect
	RECT rect;

	//Set the rect.
	rect.left	= label->m_Rect.left;
	rect.top	= label->m_Rect.top;
	rect.right	= label->m_Rect.right;
	rect.bottom = label->m_Rect.bottom;


	//Check to see if label has text.... if so draw text.. if not display the image.

	if(!strcmp(label->m_Text,"><><><"))
	{

	//Load the image.

		D3DXMATRIX scaleM;
		D3DXMatrixScaling(&scaleM, label->m_Scale.x, label->m_Scale.y, label->m_Scale.z);

		D3DXMATRIX rotM;
		D3DXMatrixRotationZ(&rotM, label->m_Rotation);

		D3DXMATRIX transM;
		D3DXMatrixTranslation(&transM, label->GetX(), label->GetY(), label->GetZ());

		D3DXMATRIX localWorld = scaleM *rotM * transM ;

		m_pSprite->SetTransform(&localWorld);


		if ( SUCCEEDED( m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK ) ) )
		{
			
			m_pSprite->Draw(
				m_Texture,
				&rect, 
				&D3DXVECTOR3( (float) (m_ImageInfo.Width/2), (float) (m_ImageInfo.Height/2), 0.0f ),
				NULL,
				0xffffffff );

			m_pSprite->End();

		}

	}
	else
	{
		DrawText(label);
	}

}


void CBitmaps2D::DrawText(CLabel* label)
{
	//Create a rect
	RECT rect;

	//Set the rect.
	rect.left	= label->m_Rect.left;
	rect.top	= label->m_Rect.top;
	rect.right	= label->m_Rect.right;
	rect.bottom = label->m_Rect.bottom;

	long fontWidth = label->m_FontWidth;


	rect.right = rect.left + fontWidth;

	float posx, posy; 

	posx = 0.0f;
	posy = 0.0f;

	D3DXMATRIX scaleM;
	D3DXMatrixScaling(&scaleM, label->m_Scale.x, label->m_Scale.y, label->m_Scale.z);

	D3DXMATRIX rotM;
	D3DXMatrixRotationZ(&rotM, label->m_Rotation);

	D3DXMATRIX transM;
	D3DXMatrixTranslation(&transM, label->GetX(), label->GetY(), label->GetZ());

	D3DXMATRIX localWorld = scaleM *rotM * transM ;

	m_pSprite->SetTransform(&localWorld);



	//Load the image.
	if ( SUCCEEDED( m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK ) ) )
	{
		for(unsigned int i = 0; i < strlen(label->m_Text); i++)
		{
			//Reset the rect.
			rect.left = label->m_Rect.left;
			rect.right = rect.left + fontWidth;

			switch(label->m_Text[i])
			{


			case 'a':
			case 'A':
				{

					//Rect should be on a.

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'b':
			case 'B':
				{
					//Shift over one.
					rect.left  += fontWidth;
					rect.right += fontWidth;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}
			case 'c':
			case 'C':
				{
					//Shift over two.
					rect.left  += fontWidth * 2;
					rect.right += fontWidth * 2;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'd':
			case 'D':
				{
					//Shift over.
					rect.left  += fontWidth * 3;
					rect.right += fontWidth * 3;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'e':
			case 'E':
				{
					//Shift over.
					rect.left  += fontWidth * 4;
					rect.right += fontWidth * 4;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'f':
			case 'F':
				{
					//Shift over.
					rect.left  += fontWidth * 5;
					rect.right += fontWidth * 5;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'g':
			case 'G':
				{
					//Shift over.
					rect.left  += fontWidth * 6;
					rect.right += fontWidth * 6;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'h':
			case 'H':
				{
					//Shift over.
					rect.left  += fontWidth * 7;
					rect.right += fontWidth * 7;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'i':
			case 'I':
				{
					//Shift over.
					rect.left  += fontWidth * 8;
					rect.right += fontWidth * 8;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'j':
			case 'J':
				{

					//Shift over.
					rect.left  += fontWidth * 9;
					rect.right += fontWidth * 9;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'k':
			case 'K':
				{
					//Shift over.
					rect.left  += fontWidth * 10;
					rect.right += fontWidth * 10;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'l':
			case 'L':
				{
					//Shift over.
					rect.left  += fontWidth * 11;
					rect.right += fontWidth * 11;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'm':
			case 'M':
				{
					//Shift over.
					rect.left  += fontWidth * 12;
					rect.right += fontWidth * 12;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'n':
			case 'N':
				{
					//Shift over.
					rect.left  += fontWidth * 13;
					rect.right += fontWidth * 13;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'o':
			case 'O':
				{
					//Shift over.
					rect.left  += fontWidth * 14;
					rect.right += fontWidth * 14;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'p':
			case 'P':
				{
					//Shift over.
					rect.left  += fontWidth * 15;
					rect.right += fontWidth * 15;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'q':
			case 'Q':
				{
					//Shift over.
					rect.left  += fontWidth * 16;
					rect.right += fontWidth * 16;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'r':
			case 'R':
				{
					//Shift over.
					rect.left  += fontWidth * 17;
					rect.right += fontWidth * 17;


					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 's':
			case 'S':
				{
					//Shift over.
					rect.left  += fontWidth * 18;
					rect.right += fontWidth * 18;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 't':
			case 'T':
				{
					//Shift over.
					rect.left  += fontWidth * 19;
					rect.right += fontWidth * 19;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'u':
			case 'U':
				{
					//Shift over.
					rect.left  += fontWidth * 20;
					rect.right += fontWidth * 20;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'v':
			case 'V':
				{
					//Shift over.
					rect.left  += fontWidth * 21;
					rect.right += fontWidth * 21;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'w':
			case 'W':
				{
					//Shift over.
					rect.left  += fontWidth * 22;
					rect.right += fontWidth * 22;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'x':
			case 'X':
				{
					//Shift over.
					rect.left  += fontWidth * 23;
					rect.right += fontWidth * 23;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'y':
			case 'Y':
				{
					//Shift over.
					rect.left  += fontWidth * 24;
					rect.right += fontWidth * 24;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case 'z':
			case 'Z':
				{
					//Shift over.
					rect.left  += fontWidth * 25;
					rect.right += fontWidth * 25;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}


			case '0':
				{
					//Shift over.
					rect.left  += fontWidth * 26;
					rect.right += fontWidth * 26;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}


			case '1':
				{
					//Shift over.
					rect.left  += fontWidth * 27;
					rect.right += fontWidth * 27;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case '2':
				{
					//Shift over.
					rect.left  += fontWidth * 28;
					rect.right += fontWidth * 28;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}


			case '3':
				{
					//Shift over.
					rect.left  += fontWidth * 29;
					rect.right += fontWidth * 29;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}


			case '4':
				{
					//Shift over.
					rect.left  += fontWidth * 30;
					rect.right += fontWidth * 30;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}


			case '5':
				{
					//Shift over.
					rect.left  += fontWidth * 31;
					rect.right += fontWidth * 31;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case '6':
				{
					//Shift over.
					rect.left  += fontWidth * 32;
					rect.right += fontWidth * 32;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case '7':
				{
					//Shift over.
					rect.left  += fontWidth * 33;
					rect.right += fontWidth * 33;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case '8':
				{
					//Shift over.
					rect.left  += fontWidth * 34;
					rect.right += fontWidth * 34;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			case '9':
				{
					//Shift over.
					rect.left  += fontWidth * 35;
					rect.right += fontWidth * 35;

					m_pSprite->Draw(
						m_Texture,
						&rect,
						NULL,
						&D3DXVECTOR3( posx, posy, 0.0f ),
						0xffffffff );
					break;
				}

			default:
				{
					//Do nothing... should update the position.. and create spacebar... MOTHER FUCKERS!!!
					//Alwayse read the fucking comments.. hey what teh FUCK are you doing in the MOHTER FUCKING 2doverlay.h for?
					// are you having fun

				}
				break;

			}


			posx += (float) fontWidth;
		 	//label->m_Rect.right += (float)fontWidth;
			//label->m_MyBody.centerPosition = (float)(label->m_Rect.left + (label->m_Rect.right/2));





		}

m_pSprite->End();

//Update the rect
//label->m_Rect.right = label->m_Rect.left + posx;
//
////And Center position.
//label->m_MyBody.centerPosition.x  =  (float)(label->m_Rect.right / 2);


	}

}
