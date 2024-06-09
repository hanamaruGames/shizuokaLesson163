#pragma once
#include "GameObject.h"
#include "FbxMesh.h"

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

	void SetPosition(VECTOR3 pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(VECTOR3 pos);
	void SetRotation(float x, float y, float z);
	void SetScale(VECTOR3 pos);
	void SetScale(float x, float y, float z);

	virtual SphereCollider Collider();
protected:
	CFbxMesh* mesh;
	Transform transform;
};