#pragma once
#include "GameObject.h"

class BG : public GameObject {
public:
	BG();
	~BG();
	void Update();
	void Draw();
private:
	CSpriteImage* image;
};