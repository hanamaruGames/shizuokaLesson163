#include "Object3D.h"

Object3D::Object3D()
{
	mesh = nullptr;
}

Object3D::~Object3D()
{
	if (mesh != nullptr) {
		delete mesh;
	}
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
	MATRIX4X4 scale = XMMatrixScaling(
		transform.scale.x, transform.scale.y,
		transform.scale.z);
	MATRIX4X4 rotX = XMMatrixRotationX(
						transform.rotation.x);
	MATRIX4X4 rotY = XMMatrixRotationY(
						transform.rotation.y);
	MATRIX4X4 rotZ = XMMatrixRotationZ(
						transform.rotation.z);
	MATRIX4X4 trans = XMMatrixTranslation(
		transform.position.x, transform.position.y,
		transform.position.z);
	MATRIX4X4 m = scale * rotZ * rotX * rotY * trans;
	mesh->Render(m);
}

void Object3D::SetPosition(VECTOR3 pos)
{
	transform.position = pos;
}

void Object3D::SetPosition(float x, float y, float z)
{
	SetPosition(VECTOR3(x, y, z));
}

void Object3D::SetRotation(VECTOR3 rot)
{
	transform.rotation = rot;
}

void Object3D::SetRotation(float x, float y, float z)
{
	SetRotation(VECTOR3(x, y, z));
}

void Object3D::SetScale(VECTOR3 sca)
{
	transform.scale = sca;
}

void Object3D::SetScale(float x, float y, float z)
{
	SetScale(VECTOR3(x, y, z));
}

SphereCollider Object3D::Collider()
{
	return SphereCollider();
}
