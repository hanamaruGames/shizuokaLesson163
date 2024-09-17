#include "PlayScene.h"
#include <assert.h>
#include "Camera.h"
#include "Ground.h"
#include "Tank.h"
#include "Barrel.h"
#include "Reticle.h"
#include "FireSplash.h"

PlayScene::PlayScene()
{
	new Ground();
	new Tank();
	new Camera();
	new Barrel(VECTOR3(20, 0, 0));
	new Barrel(VECTOR3(30, 0, 0));
	new Barrel(VECTOR3(40, 0, 0));
	new Barrel(VECTOR3(50, 0, 0));
	new Barrel(VECTOR3(60, 0, 0));
	new Reticle();
	new FireSplash();
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
