#pragma once
// Player.h
#include "Object3D.h"

class Player : public Object3D {
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
private:
	bool jumping; // �W�����v���Ă���(�Q�i�W�����v����ɂ́Aint�^�ɂ��āA0,1,2������j
	float velocityY; // Y�����̈ړ���
};