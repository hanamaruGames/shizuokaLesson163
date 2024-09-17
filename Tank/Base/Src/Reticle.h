#pragma once
#include "Object3D.h"

class Reticle : public Object3D {
public:
	Reticle();
	~Reticle();
	void Draw() override;

	VECTOR3 GetWorldPos(); // レティクルの位置を3D座標にした場所

private:
	CSpriteImage* image;
	CSprite* sprite;
};