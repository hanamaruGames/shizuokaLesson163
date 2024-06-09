#pragma once
#include "Object3D.h"
#include "Animator.h"

class Dancer : public Object3D {
public:
	Dancer();
	~Dancer();
	void Update() override;
	SphereCollider Collider() override;
private:
	Animator* animator;
};