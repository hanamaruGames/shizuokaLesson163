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
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_SPACE))
		viewTimer += 0.3f;
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
		int t = (viewTimer / 0.3f); // 表示する文字数
		if (t >= letters.size())
			t = letters.size();
		int x = 200;
		for (int i = 0; i < t; i++) {
			GameDevice()->m_pFont->Draw(x, 600, letters[i].c_str(), 
						32, RGB(255, 255, 255));
			x += letters[i].size() * 18;
		}
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
