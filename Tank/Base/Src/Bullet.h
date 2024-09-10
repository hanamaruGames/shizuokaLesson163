#pragma once
#include "Object3D.h"
#include "Barrel.h"

class Bullet : public Object3D {
public:
	Bullet(VECTOR3 from, VECTOR3 direction);
	~Bullet();
	void Update() override;
private:
	VECTOR3 velocity;
	int destroyCount; // Destroy‚³‚ê‚é‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
	Barrel* hitBarrel;
};