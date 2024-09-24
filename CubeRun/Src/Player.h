#pragma once
// Player.h
#include "Object3D.h"

class Player : public Object3D {
public:
	Player();
	~Player();
	void Update() override;
	void Draw() override;
};