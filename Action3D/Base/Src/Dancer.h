#pragma once
#include "Object3D.h"
#include "Animator.h"

class Dancer : public Object3D {
public:
	Dancer();
	~Dancer();
	void Update() override;
	SphereCollider Collider() override;
	void AddDamage(float damage, VECTOR3 pPos);
private:
	Animator* animator;
	float hitPoint;

	enum State {
		sNormal = 0, // 通常状態
		sDead, // 死亡
	};
	State state;
	void updateNormal();
	void updateDead();
};