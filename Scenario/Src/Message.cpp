#include "Message.h"

Message::Message()
{
	text = "";
	frame = new CSpriteImage(GameDevice()->m_pShader,
		"Assets/UI/box_blue_name.png");
}

Message::~Message()
{
}

void Message::Update()
{
	viewTimer += SceneManager::DeltaTime();
}

void Message::Draw()
{
	// メッセージ枠
	CSprite* spr = new CSprite();
	int x = frame->m_dwImageWidth;
	int y = frame->m_dwImageHeight;
	spr->Draw(frame, 170, 550, 0, 0, x, y);

	// テキスト
	if (text != "") {
		int t = (viewTimer / 0.5f); // 表示する文字数
		GameDevice()->m_pFont->Draw(200, 600, letters[0].c_str(), 32, RGB(255, 255, 255));
		GameDevice()->m_pFont->Draw(218, 600, letters[1].c_str(), 32, RGB(255, 255, 255));
	}
}

void Message::SetText(std::string t)
{
	viewTimer = 0.0f;
	text = t;
	letters.clear();
	for (int i = 0; i < t.size(); i++) {
		char c = t[i]; // 1文字チェック
		if (c >= 0x00 && c <= 0x7F) {
			std::string letter = t.substr(i, 1);
			letters.push_back(letter);
		}
		else {
			std::string letter = t.substr(i, 2);
			letters.push_back(letter);
			i += 1;
		}
	}
}
