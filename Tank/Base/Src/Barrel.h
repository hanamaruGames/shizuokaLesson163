#pragma once
#include "Object3D.h"

class Barrel : public Object3D {
public:
	Barrel(VECTOR3 pos); // ���W�������炤
	~Barrel();
//	void Update() override; // �����Ȃ��̂ŁA�v��Ȃ�
//	void Draw() override; // Object3D��Draw���g���̂ŁA�v��Ȃ�

	void OnCollision(Object3D* other); // �����������ɌĂ�ł��炤
};
