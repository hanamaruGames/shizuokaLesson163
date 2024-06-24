#pragma once
#include "GameObject.h"
#include "FbxMesh.h"
#include "MeshCollider.h"

class Transform {
public:
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;
	Transform() {
		position = VECTOR3(0, 0, 0);
		rotation = VECTOR3(0, 0, 0);
		scale = VECTOR3(1, 1, 1);
	}
	MATRIX4X4 matrix() {
		MATRIX4X4 scaleM = XMMatrixScaling(
			scale.x, scale.y, scale.z);
		MATRIX4X4 rotX = XMMatrixRotationX(
			rotation.x);
		MATRIX4X4 rotY = XMMatrixRotationY(
			rotation.y);
		MATRIX4X4 rotZ = XMMatrixRotationZ(
			rotation.z);
		MATRIX4X4 trans = XMMatrixTranslation(
			position.x, position.y, position.z);
		return scaleM * rotZ * rotX * rotY * trans;
	}
};

class SphereCollider {
public:
	VECTOR3 center;
	float radius;
	SphereCollider() {
		center = VECTOR3(0, 0, 0);
		radius = 0.0f;
	}
};

class Object3D : public GameObject {
public:
	Object3D();
	virtual ~Object3D();
	virtual void Update() override;
	virtual void Draw() override;

	VECTOR3 Position() {
		return transform.position;
	};
	VECTOR3 Rotation() {
		return transform.rotation;
	};
	VECTOR3 Scale() {
		return transform.scale;
	};

	void SetPosition(VECTOR3 pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(VECTOR3 pos);
	void SetRotation(float x, float y, float z);
	void SetScale(VECTOR3 pos);
	void SetScale(float x, float y, float z);

	virtual SphereCollider Collider();

	virtual bool HitSphereToMesh(const VECTOR3& center, float radius, VECTOR3* push = nullptr);
	virtual bool HitLineToMesh(const VECTOR3& from, const VECTOR3& to, VECTOR3* hit = nullptr);
protected:
	CFbxMesh* mesh;
	MeshCollider* meshCol;
	Transform transform;
};