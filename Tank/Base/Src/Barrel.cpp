#include "Barrel.h"

Barrel::Barrel(VECTOR3 pos)
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/barrel_001.mesh");
	transform.position = pos;
	transform.scale = VECTOR3(2, 2, 2);
}

Barrel::~Barrel()
{
}
