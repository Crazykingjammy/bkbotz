#ifndef __Parimitives_h__
#define __Parimitives_h__

#include "D3dlibs.h"


class CTeapot
{
private:

	// Mesh and material.
	LPD3DXMESH Mesh;
	D3DMATERIAL9 Material;


public:
	CTeapot();
	~CTeapot();

	void Initalize(IDirect3DDevice9* device, float r, float g, float b);
	void Draw(IDirect3DDevice9* device);
	void Cleanup();

};




#endif