#pragma once
// Message.h
#include "GameObject.h"

class Message : public GameObject {
public:
	Message();
	~Message();
	void Update();
	void Draw();
	void SetText(std::string t);
private:
	CSpriteImage* frame;
	std::string text;
	// textを１文字ずつ保管する
	std::vector<std::string> letters;
	float viewTimer; // 表示時間を管理
};
