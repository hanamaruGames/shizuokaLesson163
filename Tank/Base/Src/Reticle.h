#pragma once
#include "Object3D.h"

class Reticle : public Object3D {
public:
	Reticle();
	~Reticle();
	void Draw() override;
private:
	CSpriteImage* image;
	CSprite* sprite;
};