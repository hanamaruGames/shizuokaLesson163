#include "FireSplash.h"

FireSplash::FireSplash()
{
	// 画像ファイルをロード
	LoadTexture("Data/Image/particle2.png", VECTOR2(1, 1));
	transform.position = VECTOR3(0, 0, 0);
	transform.scale = VECTOR3(1, 1, 1);
	// チップを追加(後で説明)
	Chip c;
	chips.push_back(c); // 追加
	ChipControl con;
	con.velocity = VECTOR3(0.01f, 0.1f, 0.0f);
	controls.push_back(con);
	useAddBlend = true;
	useAddBlend = true; // 加算半透明

}

FireSplash::~FireSplash()
{
}

void FireSplash::Update()
{
	Chip c;
	c.scale = 0.4f;
	c.alpha = 0.9f;
	chips.push_back(c); // 追加
	ChipControl con;

	// velocity を乱数で決める
	// rand() ０～32767の乱数
	float rad = (float)rand() / 32767 * XM_2PI;
	float len = (float)rand() / 327670/3;
	con.velocity = VECTOR3(cosf(rad)*len, 0.3f, sinf(rad)*len);
	controls.push_back(con);

	// list wo mawasu
//	for (Chip& c : chips) {
//		c.offset.y += 0.01f;
//	}
//	for (std::list<Chip>::iterator itr = chips.begin(); itr != chips.end(); itr++) {
//		(*itr).offset.y += 0.01f;
//	}
	auto itrCon = controls.begin();
	for (auto itr = chips.begin(); itr != chips.end(); ) {
		(*itr).offset += (*itrCon).velocity;
		(*itrCon).velocity.y -= 0.01f;
		if ((*itr).offset.y < 0.0f) {
			// listから消す
			itr = chips.erase(itr); // ここを消して、次を返す
			itrCon = controls.erase(itrCon);
		}
		else {
			itr++;
			itrCon++;
		}
	}
}
