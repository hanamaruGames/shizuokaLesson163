#include "PlayScene.h"
#include "BG.h"
#include "ScriptExec.h"
#include "Fader.h"

PlayScene::PlayScene()
{
	Fader* f = Instantiate<Fader>();
//	f->FadeIn(2.0f); // 始まる時はフェードインする
	Instantiate<BG>();
	Instantiate<ScriptExec>();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
	GameDevice()->m_pFont->Draw(400, 15, _T("PLAY SCENE"), 16, RGB(255, 0, 0));
}