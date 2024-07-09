#include "Chara.h"

Chara::Chara()
{
	image = nullptr;
	time = 0;
	totalTime = 0;
}

Chara::~Chara()
{
}

void Chara::Update()
{
	if (time < totalTime) {
		time += SceneManager::DeltaTime();
		float rate = time / totalTime;
		if (rate > 1.0f)
			rate = 1.0f;
		position = (end - start) * rate + start;
	}
}

void Chara::Draw()
{
	if (image != nullptr) {
		// ƒLƒƒƒ‰‚ğ•`‰æ
		CSprite* spr = new CSprite();
		int x = image->m_dwImageWidth;
		int y = image->m_dwImageHeight;
		spr->Draw(image, position.x, position.y, 0, 0, x, y);
	}
}

void Chara::Set(std::string filename, int x, int y)
{
	if (image != nullptr) {
		SAFE_DELETE(image);
//		delete image;
//		image = nullptr;
	}
	std::string folder = "Assets/chara/";
	image = new CSpriteImage(GameDevice()->m_pShader,
		(folder + filename).c_str());
	position = VECTOR2(x, y);
}

void Chara::Move(int x, int y, float t)
{
	// ˆÚ“®‚É•K—v‚È•Ï”‚É‘ã“ü
	start = position;
	end = VECTOR2(x, y);
	totalTime = t;
	time = 0.0f;
}
