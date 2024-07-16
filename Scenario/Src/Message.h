#pragma once
// Message.h
#include "GameObject.h"

/// <summary>
/// ���b�Z�[�W�E�B���h�E�ɕ\������
/// �e�L�X�g�\����Ƀ{�^�����͂�҂�
/// </summary>
class Message : public GameObject {
public:
	Message();
	~Message();
	void Update();
	void Draw();
	void SetText(std::string t); // �w������
	bool IsFinish(); // �I�������true
private:
	CSpriteImage* frame;
	std::string text;
	// text���P�������ۊǂ���
	std::vector<std::string> letters;
	float viewTimer; // �\�����Ԃ��Ǘ�
	int state; // 0:�e�L�X�g�\�����A1:���͑҂��A2:���͍ς�
	int nextCounter; // �{�^�������̃A�C�R���̃J�E���^�[
};
