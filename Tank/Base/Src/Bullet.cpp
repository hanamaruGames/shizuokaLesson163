#include "Bullet.h"

Bullet::Bullet(VECTOR3 from, VECTOR3 direction)
{
	transform.position = from;
	velocity = direction;

	mesh = new CFbxMesh();
	mesh->Load("Data/Tank/bullet.mesh");
	transform.scale = VECTOR3(0.5, 0.5, 0.5);
}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
	VECTOR3 begin = transform.position;

	transform.position += velocity;
	VECTOR3 end = transform.position;
}
