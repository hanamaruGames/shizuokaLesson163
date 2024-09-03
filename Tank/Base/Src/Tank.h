#pragma once
#include "Object3D.h"

class TankGun : public Object3D {
public:
	TankGun(Object3D* parentModel);
	~TankGun();
	void Update() override;
private:
	Object3D* parent;
};

class TankTower : public Object3D {
public:
	TankTower(Object3D * parentModel);
	~TankTower();
	void Update() override;
private:
	Object3D* parent;
	VECTOR3 localRotation; // ©•ª‚Ì‰ñ“]
};

class Tank : public Object3D {
public:
	Tank();
	~Tank();
	void Update() override;
//	void Draw() override; Œp³Œ³(Object3D)‚ÌDraw()‚ğg‚¤‚©‚ç
};