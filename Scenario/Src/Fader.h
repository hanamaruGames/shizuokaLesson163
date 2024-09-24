#pragma once
// Fader.h
#include "GameObject.h"

/// <summary>
/// Fade In / Out���Ǘ�����
/// </summary>
class Fader : public GameObject {
public:
	Fader();
	~Fader();
	void Update() override;
	void Draw() override;

	void FadeOut(float seconds);
	void FadeIn(float seconds);
	// �F�w��(�F)
	void SetColor(int color);
	// �t���b�V��(�F, ����);
	void Flush(int color, float time = 1.0 / 60.0f);

private:
	// ���Ԃ𑪂�ϐ�
	float timer;
	float maxTime;

	// �����v�Z���邽�߂̕ϐ�
	//fadein: 1.0->0.0
	bool isIn;

	// �v�Z��������ۑ�����ϐ�
	float alpha;
};