#include "PlayScene.h"
#include <assert.h>
#include "Camera.h"
#include "Ground.h"
#include "Tank.h"

PlayScene::PlayScene()
{
	new Ground();
	new Tank();
	new Camera();
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
