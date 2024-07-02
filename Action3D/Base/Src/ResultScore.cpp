#include "ResultScore.h"
#include "Score.h"

ResultScore::ResultScore()
{
}

ResultScore::~ResultScore()
{
}

void ResultScore::Update()
{
}

void ResultScore::Draw()
{
	Score* sc = ObjectManager::FindGameObject<Score>();
	char str[64]; // •¶Žš—ñ‚ð—pˆÓ
	sprintf_s<64>(str, "SCORE: %6d", sc->GetScore());
	GameDevice()->m_pFont->Draw(
		600, 200, str, 64, RGB(255, 255, 255));

}
