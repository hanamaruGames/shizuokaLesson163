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
	// text‚ğ‚P•¶š‚¸‚Â•ÛŠÇ‚·‚é
	std::vector<std::string> letters;
	float viewTimer; // •\¦ŠÔ‚ğŠÇ—
};
