#pragma once
#include "GameObject.h"

class ResultScore : public GameObject {
public:
	ResultScore();
	~ResultScore();
	void Update() override;
	void Draw() override;
private:
	int viewScore; // �\������X�R�A
	float timer;
};