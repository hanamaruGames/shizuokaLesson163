#pragma once
// Fader.h
#include "GameObject.h"

/// <summary>
/// Fade In / Outを管理する
/// </summary>
class Fader : public GameObject {
public:
	Fader();
	~Fader();
	void Update() override;
	void Draw() override;

	void FadeOut(float seconds);
	void FadeIn(float seconds);
	// 色指定(色)
	void SetColor(int color);
	// フラッシュ(色, 時間);
	void Flush(int color, float time = 1.0 / 60.0f);

private:
	// 時間を測る変数
	float timer;
	float maxTime;

	// αを計算するための変数
	//fadein: 1.0->0.0
	bool isIn;

	// 計算したαを保存する変数
	float alpha;
};