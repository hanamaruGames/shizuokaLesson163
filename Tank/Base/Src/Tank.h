#pragma once
#include "Object3D.h"

class TankTower : public Object3D {
public:
	TankTower(Object3D * parentModel);
	~TankTower();
	void Update() override;
private:
	Object3D* parent;
	VECTOR3 localRotation; // �����̉�]
};

class Tank : public Object3D {
public:
	Tank();
	~Tank();
	void Update() override;
//	void Draw() override; �p����(Object3D)��Draw()���g������
};