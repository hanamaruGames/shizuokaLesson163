#include "Fader.h"

#ifdef _DEBUG // ビルドスイッチがDebugのとき
#define MAX_ALPHA 0.8f // テスト用に真っ暗にならないように
#else
#define MAX_ALPHA 1.0f
#endif

Fader::Fader()
{
	SetDrawOrder(-1000); // 一番手前に表示
	alpha = MAX_ALPHA;
	timer = 0.0f;
	maxTime = 0.0f;
	isIn = true;
}

Fader::~Fader()
{
}

void Fader::Update()
{
	if (timer < maxTime) {
		timer += SceneManager::DeltaTime();
		float rate = timer / maxTime;
		if (rate > 1.0f)
			rate = 1.0f;
		if (isIn) {
			alpha = 1.0f - rate;
		}
		else {
			alpha = rate;
		}
		if (alpha > MAX_ALPHA)
			alpha = MAX_ALPHA;
	}
}

void Fader::Draw()
{
	CSprite spr;
	spr.DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			RGB(0, 0, 0), alpha);
}

void Fader::FadeOut(float seconds)
{
	maxTime = seconds;
	timer = 0.0f;
	isIn = false;
}

void Fader::FadeIn(float seconds)
{
	maxTime = seconds;
	timer = 0.0f;
	isIn = true;
}
