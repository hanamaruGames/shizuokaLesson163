#pragma once
#include "Object3D.h"

class Bullet : public Object3D {
public:
	Bullet(VECTOR3 from, VECTOR3 direction);
	~Bullet();
	void Update() override;
private:
	VECTOR3 velocity;
};