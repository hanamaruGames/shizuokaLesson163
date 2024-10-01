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
	bool jumping; // ジャンプしている(２段ジャンプするには、int型にして、0,1,2が入る）
	float velocityY; // Y方向の移動量
};