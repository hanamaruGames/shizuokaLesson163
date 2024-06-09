#include "Door.h"

Door::Door()
{
	mesh = new CFbxMesh();
	mesh->Load("Data/MapItem/DOOR1.mesh");
}

Door::~Door()
{
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}
