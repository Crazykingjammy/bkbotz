#ifndef __Render_H__
#define __Render_H__

//Header Includes

#include "..//main.h"
#include "../Object/Object.h"

class CRender
{

public:
	
	//Default
	CRender() {}

	//Initalize
	virtual void Initalize(HINSTANCE hInstance, int Width, int Height, bool Fullscreen, HWND hwnd) = 0;

	//Update 
	virtual void Update(float timeDelta) = 0;

	//Shutdown
	virtual void Shutdown() = 0;

	//Create Scene
	virtual void CreateScene() = 0;

	//Destroy Scene
	virtual void DestoryScene() = 0;

	//Switch Camera.
	virtual void SwitchCamera() = 0;

	//Load a model and get the index
	virtual int LoadModel(char* filename) = 0;

	//Load a texture and get the index.
	virtual int LoadTexture(char* filename) = 0;

};




#endif