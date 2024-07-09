#pragma once
// Chara.h
#include "GameObject.h"

class Chara : public GameObject {
public:
	Chara();
	~Chara();
	void Update();
	void Draw();
	void Set(std::string filename, int x, int y);
	void Move(int x, int y, float time);
private:
	CSpriteImage* image;
	VECTOR2 position;

	VECTOR2 start; // 移動開始点
	VECTOR2 end; // 移動終了点
	float totalTime; // 移動時間
	float time; // 今の時間
};