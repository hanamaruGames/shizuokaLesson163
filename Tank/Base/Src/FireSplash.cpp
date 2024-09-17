#include "FireSplash.h"

FireSplash::FireSplash()
{
	// �摜�t�@�C�������[�h
	LoadTexture("Data/Image/particle2.png", VECTOR2(1, 1));
	transform.position = VECTOR3(0, 0, 0);
	transform.scale = VECTOR3(1, 1, 1);
	// �`�b�v��ǉ�(��Ő���)
	Chip c;
	chips.push_back(c); // �ǉ�
	ChipControl con;
	con.velocity = VECTOR3(0.01f, 0.1f, 0.0f);
	controls.push_back(con);
	useAddBlend = true;
	useAddBlend = true; // ���Z������

}

FireSplash::~FireSplash()
{
}

void FireSplash::Update()
{
	Chip c;
	c.scale = 0.4f;
	c.alpha = 0.9f;
	chips.push_back(c); // �ǉ�
	ChipControl con;

	// velocity �𗐐��Ō��߂�
	// rand() �O�`32767�̗���
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
			// list�������
			itr = chips.erase(itr); // �����������āA����Ԃ�
			itrCon = controls.erase(itrCon);
		}
		else {
			itr++;
			itrCon++;
		}
	}
}
