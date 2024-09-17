#include "Barrel.h"
#include "Explode.h"

Barrel::Barrel(VECTOR3 pos)
{
	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/barrel_001.mesh");

	meshCol = new MeshCollider();
	meshCol->MakeFromMesh(mesh); // Mesh‚©‚çCollider‚ðì‚é

	transform.position = pos;
	transform.scale = VECTOR3(2, 2, 2);
}

Barrel::~Barrel()
{
}

void Barrel::OnCollision(Object3D* other)
{
	Explode* ex = new Explode();
	ex->SetPosition(transform.position);
	DestroyMe();
}
