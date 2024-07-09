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

	VECTOR2 start; // �ړ��J�n�_
	VECTOR2 end; // �ړ��I���_
	float totalTime; // �ړ�����
	float time; // ���̎���
};