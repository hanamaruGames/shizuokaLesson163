#include "Explode.h"

Explode::Explode()
{
	// 画像ファイルをロード
	LoadTexture("Data/Image/Bom3.png", VECTOR2(0.25, 0.25));
	// 爆発を表示する座標
	transform.position = VECTOR3(0, 5, 0);
	transform.scale = VECTOR3(5, 5, 5);
	// チップを追加(後で説明)
	Chip c;
	chips.push_back(c);

	animTime = 0;

	useAddBlend = true; // 加算半透明
}

Explode::~Explode()
{
}

void Explode::Update()
{
	animTime++;
	int pat = animTime / 4;
	if (pat >= 16) {
		DestroyMe();
	}
	chips.back().uv = VECTOR2((pat%4)*0.25f, (pat/4)*0.25f);
	chips.back().alpha = 0.6f;
	chips.back().scale += 0.01f;
}
