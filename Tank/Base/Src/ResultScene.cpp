#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
	GameDevice()->m_pFont->Draw(
		20, 20, "ResultScene", 16, RGB(255, 0, 0));
}
