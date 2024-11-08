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
	VECTOR3 localRotation; // 自分の回転
};

class Tank : public Object3D {
public:
	Tank();
	~Tank();
	void Update() override;
//	void Draw() override; 継承元(Object3D)のDraw()を使うから
};