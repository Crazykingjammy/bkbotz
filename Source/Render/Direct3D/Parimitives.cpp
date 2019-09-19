#include "parimitives.h"


CTeapot::CTeapot()
{
	Mesh = NULL;
}

CTeapot::~CTeapot()
{
}

void CTeapot::Initalize(IDirect3DDevice9* device, float r, float g, float b)
{

	//Clean out first.
	ZeroMemory(&Material, sizeof(D3DMATERIAL9));

	//Set Diffuse
	Material.Diffuse.r = r;
	Material.Diffuse.g = g;
	Material.Diffuse.b = b;

	//Set Ambient
	Material.Ambient.r = 0.1f;
	Material.Ambient.g = 0.1f;
	Material.Ambient.b = 0.1f;

	//Set Specular
	Material.Specular.r = 0.5f;
	Material.Specular.g = 0.5f;
	Material.Specular.b = 0.5f;

	//Set Power
	Material.Power = 8.0f;


	// To create the teapot we simple call D3DXCreateTeapot and the rest is done for us.
	D3DXCreateTeapot(device, &Mesh, NULL);

}

void CTeapot::Draw(IDirect3DDevice9* device)
{
	device->SetMaterial(&Material);
	Mesh->DrawSubset(0);
}

void CTeapot::Cleanup()
{
	if(Mesh != NULL)
		Mesh->Release();

	Mesh = NULL;

}
