#include "PlayScene.h"
#include <assert.h>
#include "Camera.h"
#include "Ground.h"

PlayScene::PlayScene()
{
	new Camera();
	new Ground();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (GameDevice()->m_pDI->
		CheckKey(KD_TRG, DIK_T)) {
		SceneManager::ChangeScene("TitleScene");
	}
}

void PlayScene::Draw()
{
}
