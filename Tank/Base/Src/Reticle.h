#pragma once
#include "Object3D.h"

class Reticle : public Object3D {
public:
	Reticle();
	~Reticle();
	void Draw() override;

	VECTOR3 GetWorldPos(); // ���e�B�N���̈ʒu��3D���W�ɂ����ꏊ

private:
	CSpriteImage* image;
	CSprite* sprite;
};