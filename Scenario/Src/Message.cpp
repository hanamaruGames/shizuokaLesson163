#include "Message.h"

Message::Message()
{
	text = "";
	frame = new CSpriteImage(GameDevice()->m_pShader,
		"Assets/UI/box_blue_name.png");
	state = 2;
}

Message::~Message()
{
}

void Message::Update()
{
	viewTimer += SceneManager::DeltaTime();
	if (GameDevice()->m_pDI->CheckKey(KD_DAT, DIK_SPACE))
		viewTimer += 0.3f;
	if (state == 0) {
		int t = (viewTimer / 0.3f); // �\�����镶����
		if (t >= letters.size())
			state = 1;
	}
	else if (state == 1) {
		if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_SPACE))
			state = 2;	
	}
}

void Message::Draw()
{
	// ���b�Z�[�W�g
	CSprite* spr = new CSprite();
	int x = frame->m_dwImageWidth;
	int y = frame->m_dwImageHeight;
	spr->Draw(frame, 170, 550, 0, 0, x, y);

	// �e�L�X�g
	if (text != "" && state < 2) {
		int t = (viewTimer / 0.3f); // �\�����镶����
		if (t >= letters.size())
			t = letters.size();
		int x = 200;
		for (int i = 0; i < t; i++) {
			GameDevice()->m_pFont->Draw(x, 600, letters[i].c_str(), 
						32, RGB(255, 255, 255));
			x += letters[i].size() * 18;
		}
	}
	if (state == 1) {
		nextCounter++;
		//                           ����(60frame)    �ӂ蕝
		float d = sinf(nextCounter / 60.0f * XM_2PI) * 20.0f;
		d = abs(d); // ��Βl
		GameDevice()->m_pFont->Draw(800, 
				650-d, "��", 32,
				RGB(255, 0, 0));
	}
}

void Message::SetText(std::string t)
{
	viewTimer = 0.0f;
	text = t;
	letters.clear();
	for (int i = 0; i < t.size(); i++) {
		char c = t[i]; // 1�����`�F�b�N
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
	state = 0;
}

bool Message::IsFinish()
{
	// �e�L�X�g��\�����I�������true�ɂ���
//	int t = (viewTimer / 0.3f); // �\�����镶����
//	if (t >= letters.size()) {
	if (state==2) {
		return true;
	}
	return false;
}
