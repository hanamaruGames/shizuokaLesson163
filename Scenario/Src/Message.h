#pragma once
// Message.h
#include "GameObject.h"

/// <summary>
/// メッセージウィンドウに表示する
/// テキスト表示後にボタン入力を待つ
/// </summary>
class Message : public GameObject {
public:
	Message();
	~Message();
	void Update();
	void Draw();
	void SetText(std::string t); // 指示だし
	bool IsFinish(); // 終わったらtrue
private:
	CSpriteImage* frame;
	std::string text;
	// textを１文字ずつ保管する
	std::vector<std::string> letters;
	float viewTimer; // 表示時間を管理
	int state; // 0:テキスト表示中、1:入力待ち、2:入力済み
	int nextCounter; // ボタン押しのアイコンのカウンター
};
