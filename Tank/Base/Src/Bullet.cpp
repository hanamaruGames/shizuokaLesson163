#include "Bullet.h"

Bullet::Bullet(VECTOR3 from, VECTOR3 direction)
{
	transform.position = from;
	velocity = direction;

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/tank_gun_001.mesh");
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	transform.position += velocity;
}
